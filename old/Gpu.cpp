#include "Gpu.h"

Gpu::Gpu()
{
	framebuffer = new unsigned int[64 * 32];
	Clear();
	texture = new Texture(64, 32, framebuffer);
}

Gpu::~Gpu()
{
	delete [] framebuffer;
	delete texture;
}

void Gpu::Clear()
{
	for (int i = 0; i < 64 * 32; i++) framebuffer[i] = 0;
}

void Gpu::Update()
{
	texture->SetData(framebuffer);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->GetHandle());
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 1);
	glVertex2i(-1, -1);
	glTexCoord2i(1, 1);
	glVertex2i( 1, -1);
	glTexCoord2i(1, 0);
	glVertex2i( 1,  1);
	glTexCoord2i(0, 0);
	glVertex2i(-1,  1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

int Gpu::DrawSprite(int x, int y, unsigned char *buf, int n)
{
	int ret = 0;
	for (int i = 0; i < n; i++)
	{
		if (y >= 0 && y < 32)
		{
			unsigned char c = buf[i];
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
