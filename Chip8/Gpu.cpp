#include "Gpu.h"

Gpu::Gpu()
{
	framebuffer = new unsigned int[64 * 32];
	Clear();

	videoModule = nullptr;
}

Gpu::~Gpu()
{
	delete [] framebuffer;
}

void Gpu::SetVideoModule(VideoModule *videoModule)
{
	this->videoModule = videoModule;
}

void Gpu::Clear()
{
	for (int i = 0; i < 64 * 32; i++) framebuffer[i] = 0;
}

void Gpu::Update()
{
	if (videoModule) videoModule->SetOutput(64, 32, framebuffer);
}

int Gpu::DrawSprite(int x, int y, unsigned char *buf, int n)
{
	int ret = 0;
	for (int i = 0; i < n; i++)
	{
		if (y >= 0 && y < 32)
		{
			auto c = buf[i];
			for (int j = 0; j < 8; j++)
			{
				if ((c >> (7 - j)) & 1)
				{
					int bi = y * 64 + ((x + j) & 63);
					if (framebuffer[bi] != 0)
					{
						framebuffer[bi] = 0;
						ret = 1;
					}
					else
					{
						framebuffer[bi] = 0xffffffff;
					}
				}
			}
		}
		y++;
	}
	return ret;
}
