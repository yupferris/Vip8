#include "Apu.h"

#include <math.h>

Apu::Apu(int latencyMs)
{
	audioStream = new AudioStream(renderRouter, this, latencyMs);

	enabled = true;
	beeping = false;
	beepPhase = 0.0;
}

Apu::~Apu()
{
	delete audioStream;
}

void Apu::Update()
{
	audioStream->Update();
}

void Apu::SetLatencyMs(int latencyMs)
{
	audioStream->SetLatencyMs(latencyMs);
}

int Apu::GetLatencyMs() const
{
	return audioStream->GetLatencyMs();
}

void Apu::SetEnabled(bool enabled)
{
	this->enabled = enabled;
}

bool Apu::GetEnabled() const
{
	return enabled;
}

void Apu::SetBeeping(bool value)
{
	beeping = value;
}

void Apu::render(AudioFormat::Sample *buffer, int length)
{
	for (int i = 0; i < length; i++)
	{
		int s = -10000;
		if (enabled && beeping)
		{
			if (sin(beepPhase) > 0.0) s = 10000;
			beepPhase += .02;
		}
		buffer[i] = (AudioFormat::Sample) s;
	}
}

void Apu::renderRouter(AudioFormat::Sample *buffer, int length, void *apu)
{
	((Apu *)apu)->render(buffer, length);
}
