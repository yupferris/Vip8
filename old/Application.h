#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Common.h"
#include "List.h"
#include "GLWindow.h"
#include "Chip8.h"

class Application
{
public:
	static void Run(const List<string>& args);

private:
	static Application *get();
	
	static void windowEventHandler(Window& w, int event, WPARAM wParam, LPARAM lParam);
	void reflectZoom();
	void reflectSettingsInMenu();

	void loadAndStartRom(string fileName);

	void setWorkingDirectory();
	void loadSettings(string fileName);
	void saveSettings(string fileName);

	GLWindow *window;
	HMENU menu;
	int zoom;
	Chip8 *chip8;
};

#endif
