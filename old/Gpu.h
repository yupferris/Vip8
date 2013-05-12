#ifndef __GPU_H__
#define __GPU_H__

#include "Common.h"
#include "Texture.h"

class Gpu
{
public:
	Gpu();
	~Gpu();

	void Clear();
	void Update();
	int DrawSprite(int x, int y, unsigned char *buf, int n);

private:
	unsigned int *framebuffer;
	Texture *texture;
};

#endif
