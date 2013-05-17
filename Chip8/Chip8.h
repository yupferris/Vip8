#ifndef __CHIP8_H__
#define __CHIP8_H__

#include "../Common.h"
#include "Gpu.h"
#include "Apu.h"

#include "../IAudioDriver.h"

class Chip8 : public IEmulator
{
public:
	Chip8();
	~Chip8();

	virtual void Reset();
	virtual void Update();

	virtual int GetOutputWidth() const;
	virtual int GetOutputHeight() const;

	virtual void SetVideoDriver(IVideoDriver *videoDriver);
	virtual void SetAudioDriver(IAudioDriver *audioDriver);

	void LoadRom(const List<unsigned char>& input);
	bool HasRom() const;
	void Start();
	bool IsRunning() const;
	void SetInput(int index, bool value);
	bool GetInput(int index);

	void SetSpeed(int speed);
	int GetSpeed() const;

private:
	static void invalidOpcode();

	unsigned short pc;
	unsigned char regs[16];
	unsigned short iReg;

	unsigned char timerDelay, timerSound;
	unsigned char *ram, *rom;
	int romSize;

	static const unsigned char charMem[];

	Stack<unsigned short> *stack;
	bool inputs[16];

	Gpu gpu;
	Apu apu;

	bool running;
	bool waitingForKeypress;
	int speed;
	unsigned int timer;
	Random random;
};

#endif
