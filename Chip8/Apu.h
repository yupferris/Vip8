#ifndef __CHIP8_APU_H__
#define __CHIP8_APU_H__

#include "../Common.h"

#include "../IAudioDriver.h"

class Apu
{
public:
	Apu();
	~Apu();

	void SetAudioDriver(IAudioDriver *audioDriver);

	void SetBeeping(bool beeping);

private:
	static void renderCallbackRouter(float *leftBuffer, float *rightBuffer, int numSamples, void *userData);

	void renderCallback(float *leftBuffer, float *rightBuffer, int numSamples);

	IAudioDriver *audioDriver;
	Mutex *mutex;
	bool beeping;
	double beepPhase;
};

#endif
