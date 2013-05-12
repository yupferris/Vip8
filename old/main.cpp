// Chips8TheSalsa
//  A Chip-8 Emulator for Windows

// (C) 2012 Jake "Ferris" Taylor

#include "Common.h"
#include "List.h"
#include "Application.h"

int main(int argc, char **argv)
{
	try
	{
		List<string> args;
		for (int i = 1; i < argc; i++) args.Add(string(argv[i]));
		
		Application::Run(args);
	}
	catch (const Exception& e)
	{
		MessageBox(0, e.GetMsg().c_str(), "Chips8TheSalsa Fatal Error", MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}

	return 0;
}

#ifdef WIN32
INT WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	return main(__argc, __argv);
}
#endif
