#include "Apu.h"

Apu::Apu()
{
	mutex = Mutex::Create();
}

Apu::~Apu()
{
	delete mutex;
}

void Apu::SetAudioDriver(IAudioDriver *audioDriver)
{
}

void Apu::SetBeeping(bool beeping)
{
	mutex->Lock();
	this->beeping = beeping;
	mutex->Unlock();
}
