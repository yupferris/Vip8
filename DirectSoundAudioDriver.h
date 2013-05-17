#ifndef __DIRECTSOUNDAUDIODRIVER_H__
#define __DIRECTSOUNDAUDIODRIVER_H__

#include "Common.h"
#include "IAudioDriver.h"

#include <Windows.h>
#include <dsound.h>

class DirectSoundAudioDriver : public IAudioDriver
{
public:
	DirectSoundAudioDriver(int latencyMs = 60, int sampleRate = 44100, RenderCallback renderCallback = nullptr, void *userData = nullptr);
	virtual ~DirectSoundAudioDriver();

	virtual void SetRenderCallback(RenderCallback renderCallback, void *userData);

	virtual void SetEnabled(bool enabled);
	virtual bool GetEnabled() const;

	virtual void SetLatencyMs(int latencyMs);
	virtual int GetLatencyMs() const;

	virtual int GetSampleRate() const;

private:
	static DWORD WINAPI threadProc(LPVOID lpParameter);

	void createBuffers();
	void destroyBuffers();
	void recreateBuffers();

	void renderSamples(short *buffer, int numSamples);

	int latencyMs;
	int sampleRate;
	bool enabled;
	RenderCallback renderCallback;
	void *userData;

	LPDIRECTSOUND8 device;
	int bufferSizeBytes;
	float *leftBuffer, *rightBuffer;
	LPDIRECTSOUNDBUFFER buffer;
	int oldPlayCursorPos;

	Mutex *mutex;
	HANDLE thread;
};

#endif
