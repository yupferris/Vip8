#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Common.h"

class Texture
{
public:
	Texture(int width, int height, void *data = 0, GLenum intFormat = GL_RGBA);
	~Texture();
	GLuint GetHandle() const;
	int GetWidth() const;
	int GetHeight() const;
	void SetData(void *data, GLenum intFormat = GL_RGBA);

private:
	GLuint handle;
	int width, height;
};

#endif
