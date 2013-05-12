#ifndef __CHIP8_GPU_H__
#define __CHIP8_GPU_H__

#include "../Common.h"

class Gpu
{
public:
	Gpu();
	~Gpu();

	void SetVideoModule(VideoModule *videoModule);

	void Clear();
	void Update();
	int DrawSprite(int x, int y, unsigned char *buf, int n);

private:
	unsigned int *framebuffer;
	VideoModule *videoModule;
};

#endif
