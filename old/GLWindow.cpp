#include "GLWindow.h"

GLWindow::GLWindow(): Window()
{
	
}

GLWindow::GLWindow(string title, bool fullscreen, int w, int h, int style)
{
	Open(title, fullscreen, w, h, style);
}

GLWindow::~GLWindow()
{
	Close();
}

void GLWindow::Open(string title, bool fullscreen, int w, int h, int style)
{
	Window::Open(title, fullscreen, w, h, style);
	dc = GetDC(handle);

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0, // No alpha buffer
		0,
		0, // No accumulation buffer
		0, 0, 0, 0,
		24, // 24-bit z buffer
		0, // No stencil buffer
		0, // No auxilary buffer
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};
	SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
	rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);
}

void GLWindow::Close()
{
	if (open)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(rc);
		ReleaseDC(handle, dc);
	}
	Window::Close();
}

void GLWindow::Update()
{
	Window::Update();
	SwapBuffers(dc);
}
