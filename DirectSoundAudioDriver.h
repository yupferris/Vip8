#ifndef __DIRECTSOUNDAUDIODRIVER_H__
#define __DIRECTSOUNDAUDIODRIVER_H__

#include "Common.h"
#include "IAudioDriver.h"

#include <Windows.h>
#include <dsound.h>

class DirectSoundAudioDriver : public IAudioDriver
{
public:
	DirectSoundAudioDriver(RenderCallback renderCallback, int latencyMs = 100, int sampleRate = 44100);
	virtual ~DirectSoundAudioDriver();

	virtual void SetLatencyMs(int latencyMs);
	virtual int GetLatencyMs() const;

	virtual int GetSampleRate() const;

private:
	RenderCallback renderCallback;
	int latencyMs;
	int sampleRate;

	HANDLE thread;
};

#endif
