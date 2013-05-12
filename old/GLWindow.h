#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include "Common.h"
#include "Window.h"

class GLWindow: public Window
{
public:
	GLWindow();
	GLWindow(string title, bool fullscreen = false, int w = 800, int h = 600, int style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	~GLWindow();
	virtual void Open(string title, bool fullscreen = false, int w = 800, int h = 600, int style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	virtual void Close();
	virtual void Update();

private:
	HDC dc;
	HGLRC rc;
};

#endif
