#ifndef __CHIP8_H__
#define __CHIP8_H__

#include "../Common.h"
#include "Gpu.h"
#include "Apu.h"

class Chip8 : public IEmulator
{
public:
	Chip8();
	~Chip8();

	virtual void Reset();
	void Update();

	int GetOutputWidth() const;
	int GetOutputHeight() const;

	void SetVideoDriver(IVideoDriver *videoDriver);
	void SetAudioDriver(IAudioDriver *audioDriver);

	virtual void CpuCyclesCallback(int numCycles);

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
