#ifndef __AUDIO_STREAM__
#define __AUDIO_STREAM__

#include "Common.h"
#include "AudioFormat.h"

#include <dsound.h>

class AudioStream
{
public:
	typedef void (* RenderCallback)(AudioFormat::Sample *buffer, int length, void *userData);

	AudioStream(RenderCallback renderCallback, void *userData, int latencyMs = 100);
	~AudioStream();

	void Update();

	void SetLatencyMs(int latencyMs);
	int GetLatencyMs() const;

private:
	void init();
	void free();

	int latencyMs;
	int bufferSizeBytes;

	RenderCallback renderCallback;
	void *userData;

	LPDIRECTSOUND8 device;
	LPDIRECTSOUNDBUFFER buffer;
	int oldPlayCursorPos;
};

#endif
