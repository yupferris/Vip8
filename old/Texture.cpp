#include "Texture.h"

Texture::Texture(int width, int height, void *data, GLenum intFormat)
{
	this->width = width;
	this->height = height;
	glGenTextures(1, &handle);
	SetData(data, intFormat);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
}

GLuint Texture::GetHandle() const
{
	return handle;
}

int Texture::GetWidth() const
{
	return width;
}

int Texture::GetHeight() const
{
	return height;
}

void Texture::SetData(void *data, GLenum intFormat)
{
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, intFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}
