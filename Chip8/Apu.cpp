#include "Apu.h"

Apu::Apu()
{
	audioDriver = nullptr;
	mutex = Mutex::Create();
	beeping = false;
	beepPhase = 0.0;
}

Apu::~Apu()
{
	delete mutex;
}

void Apu::SetAudioDriver(IAudioDriver *audioDriver)
{
	mutex->Lock();
	auto oldAudioDriver = this->audioDriver;
	if (oldAudioDriver) oldAudioDriver->SetRenderCallback(nullptr, nullptr);
	this->audioDriver = audioDriver;
	audioDriver->SetRenderCallback(renderCallbackRouter, this);
	mutex->Unlock();
}

void Apu::SetBeeping(bool beeping)
{
	mutex->Lock();
	this->beeping = beeping;
	mutex->Unlock();
}

void Apu::renderCallbackRouter(float *leftBuffer, float *rightBuffer, int numSamples, void *userData)
{
	if (userData) ((Apu *)userData)->renderCallback(leftBuffer, rightBuffer, numSamples);
}

void Apu::renderCallback(float *leftBuffer, float *rightBuffer, int numSamples)
{
	mutex->Lock();

	auto beepPhaseDelta = .04 * (44100.0 / (double)audioDriver->GetSampleRate());

	for (int i = 0; i < numSamples; i++)
	{
		leftBuffer[i] = rightBuffer[i] = beeping ? (sin(beepPhase) > 0.0 ? 1.0f : -1.0f) : 0.0f;
		beepPhase += .04;
	}

	mutex->Unlock();
}
