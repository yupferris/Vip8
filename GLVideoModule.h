#ifndef __GLVIDEOMODULE_H__
#define __GLVIDEOMODULE_H__

#include "Common.h"

#include <Windows.h>
#include <gl/GL.h>

class GLVideoModule : public VideoModule
{
public:
	GLVideoModule(Viewport *viewport);
	~GLVideoModule();

	virtual void SetOutput(int width, int height, const unsigned int *data);

private:
	void SetupDC();

	Viewport *viewport;
	HWND outputHandle;
	HDC dc;
	HGLRC rc;

	GLuint textureHandle;
};

#endif
