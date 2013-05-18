#ifndef __GLVIDEODRIVER_H__
#define __GLVIDEODRIVER_H__

#include "Common.h"
#include <Fgl/Win32Viewport.h>

#include <Windows.h>
#include <gl/GL.h>

class GLVideoDriver : public IVideoDriver
{
public:
	GLVideoDriver(Viewport *viewport = nullptr);
	virtual ~GLVideoDriver();

	virtual void SetViewport(Viewport *viewport);

	virtual void SetOutput(int width, int height, const unsigned int *data);

private:
	void createDC();
	void destroyDC();

	void createRC();

	Win32Viewport *viewport;
	HWND outputHandle;
	HDC dc;
	HGLRC rc;

	GLuint textureHandle;
};

#endif
