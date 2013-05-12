#ifndef __APU_H__
#define __APU_H__

#include "Common.h"
#include "AudioFormat.h"
#include "AudioStream.h"

class Apu
{
public:
	Apu(int latencyMs = 100);
	~Apu();

	void Update();

	void SetLatencyMs(int latencyMs);
	int GetLatencyMs() const;
	void SetEnabled(bool enabled);
	bool GetEnabled() const;
	void SetBeeping(bool value);

private:
	void render(AudioFormat::Sample *buffer, int length);
	static void renderRouter(AudioFormat::Sample *buffer, int length, void *apu);

	AudioStream *audioStream;

	bool enabled;
	bool beeping;
	double beepPhase;
};

#endif
