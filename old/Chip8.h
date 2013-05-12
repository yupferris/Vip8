#ifndef __CHIP8_H__
#define __CHIP8_H__

#include "Common.h"
#include "Stack.h"
#include "Gpu.h"
#include "Apu.h"

class Chip8
{
public:
	Chip8();
	~Chip8();

	void Reset();
	void LoadRom(string fileName);
	bool HasRom() const;
	void Start();
	void Update();
	bool IsRunning() const;
	void SetInput(int index, bool value);
	bool GetInput(int index);

	void SetSpeed(int speed);
	int GetSpeed() const;
	void SetAudioEnabled(bool enabled);
	bool GetAudioEnabled() const;
	void SetAudioLatencyMs(int latencyMs);
	int GetAudioLatencyMs() const;

private:
	static void invalidOpcode();

	unsigned short pc;
	unsigned char regs[16];
	unsigned short iReg;
	unsigned char timerDelay;
	unsigned char timerSound;
	unsigned char *ram;
	unsigned char *rom;
	int romSize;
	static unsigned char charMem[];
	Stack<unsigned short> *stack;
	bool inputs[16];
	Gpu gpu;
	Apu apu;
	bool running;
	bool waitingForKeypress;
	int speed;
	unsigned int timer;
};

#endif
