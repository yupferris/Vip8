#include "GLVideoModule.h"

GLVideoModule::GLVideoModule(Viewport *viewport)
{
	this->viewport = (Win32Viewport *)viewport;
	outputHandle = this->viewport->GetHandle();
	SetupDC();
	rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLVideoModule::~GLVideoModule()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(rc);
	ReleaseDC(outputHandle, dc);
}

void GLVideoModule::SetOutput(int width, int height, const unsigned int *data)
{
	auto newOutputHandle = viewport->GetHandle();
	if (newOutputHandle != outputHandle)
	{
		wglMakeCurrent(NULL, NULL);
		ReleaseDC(outputHandle, dc);
		outputHandle = newOutputHandle;
		SetupDC();
	}
	wglMakeCurrent(dc, rc);

	glClear(GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
	glEnable(GL_TEXTURE_2D);
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
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SwapBuffers(dc);
}

void GLVideoModule::SetupDC()
{
	dc = GetDC(outputHandle);
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
		0, // No z buffer
		0, // No stencil buffer
		0, // No auxilary buffer
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};
	SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
}
