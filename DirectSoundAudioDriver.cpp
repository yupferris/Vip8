#include "DirectSoundAudioDriver.h"

DirectSoundAudioDriver::DirectSoundAudioDriver(RenderCallback renderCallback, int latencyMs, int sampleRate)
{
	if (!renderCallback) throw FSL_EXCEPTION("Render callback cannot be null");
	this->renderCallback = renderCallback;
	this->latencyMs = latencyMs;
	this->sampleRate = sampleRate;
}

DirectSoundAudioDriver::~DirectSoundAudioDriver()
{
}

void DirectSoundAudioDriver::SetLatencyMs(int latencyMs)
{
}

int DirectSoundAudioDriver::GetLatencyMs() const
{
	return latencyMs;
}

int DirectSoundAudioDriver::GetSampleRate() const
{
	return sampleRate;
}
