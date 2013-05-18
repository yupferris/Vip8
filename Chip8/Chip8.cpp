#include "Chip8.h"

const unsigned char Chip8::charMem[] =
{
	0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2
	0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
	0x90, 0x90, 0xf0, 0x10, 0x10, // 4
	0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
	0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
	0xf0, 0x10, 0x20, 0x40, 0x40, // 7
	0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
	0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
	0xf0, 0x90, 0xf0, 0x90, 0x90, // a
	0xe0, 0x90, 0xe0, 0x90, 0xe0, // b
	0xf0, 0x80, 0x80, 0x80, 0xf0, // c
	0xe0, 0x90, 0x90, 0x90, 0xe0, // d
	0xf0, 0x80, 0xf0, 0x80, 0xf0, // e
	0xf0, 0x80, 0xf0, 0x80, 0x80, // f
};

Chip8::Chip8()
{
	random.SetSeed(Time::GetCurrent());
	ram = new unsigned char[4096];
	rom = 0;
	romSize = 0;
	stack = new Stack<unsigned short>(16);
	speed = 20;
	Reset();
}

Chip8::~Chip8()
{
	delete [] ram;
	if (rom) delete rom;
	delete stack;
}

void Chip8::Reset()
{
	pc = 0x200;
	for (int i = 0; i < 16; i++) regs[i] = 0;
	iReg = 0;
	timerDelay = 0;
	timerSound = 0;
	for (int i = 0; i < 4096; i++) ram[i] = random.GetNextInt(256);
	for (int i = 0; i < 16 * 5; i++) ram[i] = charMem[i];
	for (int i = 0; i < 16; i++) inputs[i] = false;
	gpu.Clear();
	apu.SetBeeping(false);
	running = false;
	waitingForKeypress = false;
}

void Chip8::Update()
{
	if (!running) return;
	int t = 16 - (Time::GetCurrent() - timer);
	Threading::Sleep(t > 1 ? t : 1);
	timer = Time::GetCurrent();

	try
	{
		if (timerDelay > 0) timerDelay--;
		if (timerSound > 0)
		{
			timerSound--;
			apu.SetBeeping(timerSound > 0);
		}

		int keyPressReg = 0;
		for (int i = 0; i < speed; i++)
		{
			if (waitingForKeypress)
			{
				for (int j = 0; j < 16; j++)
				{
					if (GetInput(j))
					{
						regs[keyPressReg] = j;
						waitingForKeypress = false;
						break;
					}
				}
				continue;
			}

			if (pc < 0 || pc >= 4095) throw FSL_EXCEPTION("PC out of bounds");
			unsigned short opcode = (ram[pc] << 8) | ram[pc + 1];
			pc += 2;

			int h = (opcode >> 12) & 0xf;
			int x = (opcode >> 8) & 0xf;
			int y = (opcode >> 4) & 0xf;
			int n = opcode & 0xf;
			int nn = opcode & 0xff;
			int nnn = opcode & 0xfff;

			switch (h)
			{
			case 0:
				if (opcode & 0x0f00)
				{
					// SYS addr (ignored)
				}
				else if(opcode == 0x00e0)
				{
					// CLS
					gpu.Clear();
				}
				else if (opcode == 0x00ee)
				{
					// RET
					pc = stack->Pop();
				}
				else
				{
					invalidOpcode();
				}
				break;

			case 1:
				// JP addr
				pc = nnn;
				break;

			case 2:
				// CALL addr
				stack->Push(pc);
				pc = nnn;
				break;

			case 3:
				// SE Vx, byte
				if (regs[x] == nn) pc += 2;
				break;

			case 4:
				// SNE Vx, byte
				if (regs[x] != nn) pc += 2;
				break;

			case 5:
				if (n) invalidOpcode();

				// SE Vx, Vy
				if (regs[x] == regs[y]) pc += 2;
				break;

			case 6:
				// LD Vx, byte
				regs[x] = nn;
				break;

			case 7:
				// ADD Vx, byte
				regs[x] += nn;
				break;

			case 8:
				switch (opcode & 0xf)
				{
				case 0:
					// LD Vx, Vy
					regs[x] = regs[y];
					break;

				case 1:
					// OR Vx, Vy
					regs[x] |= regs[y];
					break;

				case 2:
					// AND Vx, Vy
					regs[x] &= regs[y];
					break;

				case 3:
					// XOR Vx, Vy
					regs[x] ^= regs[y];
					break;

				case 4:
					{
						// ADD Vx, Vy
						int result = regs[x] + regs[y];
						regs[15] = result > 255 ? 1 : 0;
						regs[x] = result;
					}
					break;

				case 5:
					// SUB Vx, Vy
					regs[15] = regs[x] < regs[y] ? 0 : 1;
					regs[x] -= regs[y];
					break;

				case 6:
					// SHR Vx
					regs[15] = regs[x] & 1;
					regs[x] >>= 1;
					break;

				case 7:
					// SUBN Vx, Vy
					regs[15] = regs[y] < regs[x] ? 0 : 1;
					regs[x] = regs[y] - regs[x];
					break;

				case 0xe:
					// SHL Vx
					regs[15] = (regs[x] >> 7) & 1;
					regs[x] <<= 1;
					break;

				default:
					invalidOpcode();
				}
				break;

			case 9:
				// SNE Vx, Vy
				if (regs[x] != regs[y]) pc += 2;
				break;

			case 0xa:
				// LD i, addr
				iReg = nnn;
				break;

			case 0xb:
				// JP V0, addr
				pc = nnn + regs[0];
				break;

			case 0xc:
				// RND Vx, byte
				regs[x] = random.GetNextInt() & nn;
				break;

			case 0xd:
				// DRW Vx, Vy, nibble
				regs[15] = gpu.DrawSprite(regs[x], regs[y], ram + iReg, n);
				break;

			case 0xe:
				if (nn == 0x9e)
				{
					// SKP Vx
					if (GetInput(regs[x])) pc += 2;
				}
				else if (nn == 0xa1)
				{
					// SKNP Vx
					if (!GetInput(regs[x])) pc += 2;
				}
				else
				{
					invalidOpcode();
				}
				break;

			case 0xf:
				switch (nn)
				{
				case 0x07:
					// LD Vx, DT
					regs[x] = timerDelay;
					break;

				case 0x0a:
					// LD Vx, K
					keyPressReg = x;
					waitingForKeypress = true;
					break;

				case 0x15:
					// LD DT, Vx
					timerDelay = regs[x];
					break;

				case 0x18:
					// LD ST, Vx
					timerSound = regs[x];
					break;

				case 0x1e:
					{
						// ADD I, Vx
						auto result = iReg + regs[x];
						regs[15] = result > 0xfff ? 1 : 0;
						iReg = result & 0xfff;
					}
					break;

				case 0x29:
					// LD F, Vx
					iReg = regs[x] * 5;
					break;

				case 0x33:
					{
						// LD B, Vx
						char c = regs[x];
						ram[(iReg + 2) & 0xfff] = c % 10;
						ram[(iReg + 1) & 0xfff] = (c / 10) % 10;
						ram[iReg] = (c / 100) % 10;
					}
					break;

				case 0x55:
					{
						// LD [I], Vx
						for (int i = 0; i <= x; i++) ram[(iReg + i) & 0xfff] = regs[i];
						iReg = (iReg + x + 1) & 0xfff;
					}
					break;

				case 0x65:
					{
						// LD Vx, [I]
						for (int i = 0; i <= x; i++) regs[i] = ram[(iReg + i) & 0xfff];
						iReg = (iReg + x + 1) & 0xfff;
					}
					break;

				default:
					invalidOpcode();
				}
				break;

			default:
				invalidOpcode();
			}
		}
	}
	catch (const Exception& e)
	{
		MessageWindow::Error(e.GetMsg());
		running = false;
	}

	gpu.Update();
}

int Chip8::GetOutputWidth() const
{
	return 64;
}

int Chip8::GetOutputHeight() const
{
	return 32;
}

void Chip8::SetVideoDriver(IVideoDriver *videoDriver)
{
	gpu.SetVideoDriver(videoDriver);
}

void Chip8::SetAudioDriver(IAudioDriver *audioDriver)
{
	apu.SetAudioDriver(audioDriver);
}

void Chip8::LoadRom(const List<unsigned char>& input)
{
	if (input.Count() > 3584) throw FSL_EXCEPTION("ROM image is too large");
	romSize = input.Count();
	if (rom) delete rom;
	rom = new unsigned char[romSize];
	for (int i = 0; i < romSize; i++) rom[i] = input[i];
}

bool Chip8::HasRom() const
{
	return rom != 0;
}

void Chip8::Start()
{
	if (running) throw FSL_EXCEPTION("Emulator already running");
	if (rom)
	{
		for (int i = 0; i < romSize; i++) ram[i + 0x200] = rom[i];
	}
	running = true;
	timer = Time::GetCurrent();
}

bool Chip8::IsRunning() const
{
	return running;
}

void Chip8::SetInput(int index, bool value)
{
	inputs[index & 0xf] = value;
}

bool Chip8::GetInput(int index)
{
	return inputs[index & 0xf];
}

void Chip8::SetSpeed(int speed)
{
	this->speed = speed;
}

int Chip8::GetSpeed() const
{
	return speed;
}

void Chip8::invalidOpcode()
{
	throw FSL_EXCEPTION("Invalid opcode");
}
