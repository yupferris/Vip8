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
	IAudioDriver *audioDriver;
	Mutex *mutex;
	bool beeping;
};

#endif
