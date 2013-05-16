#ifndef __GLVIDEODRIVER_H__
#define __GLVIDEODRIVER_H__

#include "Common.h"
#include <Fgl/Win32Viewport.h>

#include <Windows.h>
#include <gl/GL.h>

// TODO: This should be separated.
class GLVideoDriver : public IVideoDriver
{
public:
	GLVideoDriver(Viewport *viewport);
	virtual ~GLVideoDriver();

	virtual void SetOutput(int width, int height, const unsigned int *data);

private:
	void SetupDC();

	Win32Viewport *viewport;
	HWND outputHandle;
	HDC dc;
	HGLRC rc;

	GLuint textureHandle;
};

#endif
