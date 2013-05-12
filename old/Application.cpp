#include "Application.h"

#include "resource.h"

void Application::Run(const List<string>& args)
{
	Application *a = get();

	a->window = new GLWindow("Chips8TheSalsa", false);
	a->window->AddEventHandler(windowEventHandler);
	a->menu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
	a->window->SetMenu(a->menu);

	a->zoom = 8;
	a->reflectZoom();

	a->chip8 = new Chip8();

	if (args.Count() >= 1) a->loadAndStartRom(args[0]);

	const string cfgFileName("config.bin");
	a->loadSettings(cfgFileName);

	a->reflectSettingsInMenu();

	while (a->window->IsOpen())
	{
		glViewport(0, 0, a->window->GetWidth(), a->window->GetHeight());

		a->chip8->Update();
		a->window->Update();
		Sleep(1);
	}

	a->saveSettings(cfgFileName);
}

Application *Application::get()
{
	static Application a;
	return &a;
}

void Application::windowEventHandler(Window& w, int event, WPARAM wParam, LPARAM lParam)
{
	Application *a = get();

	static char keyChars[] = { 'X', '1', '2', '3', 'Q', 'W', 'E', 'A', 'S', 'D', 'Z', 'C', '4', 'R', 'F', 'V' };

	switch (event)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_LOADROM:
			{
				char *fileName = new char[MAX_PATH];
				for (int i = 0; i < MAX_PATH; i++) fileName[i] = 0;
				OPENFILENAME ofn =
				{
					sizeof(OPENFILENAME),
					a->window->GetHandle(),
					0,
					"All Files (*.*)\0*.*\0",
					0, 0,
					1,
					fileName,
					MAX_PATH,
					0, 0, 0,
					"Load ROM Image",
					0, 0, 0, 0, 0, 0, 0, 0, 0
				};
				if (GetOpenFileName(&ofn))
				{
					a->loadAndStartRom(fileName);
				}
				delete [] fileName;
			}
			break;

		case ID_FILE_EXIT:
			a->window->Close();
			break;

		case ID_SYSTEM_RESET:
			a->chip8->Reset();
			if (a->chip8->HasRom()) a->chip8->Start();
			break;

		case ID_AUDIO_ENABLED:
			a->chip8->SetAudioEnabled(!a->chip8->GetAudioEnabled());
			break;

		case ID_LATENCY_50MS:
			a->chip8->SetAudioLatencyMs(50);
			break;

		case ID_LATENCY_100MS:
			a->chip8->SetAudioLatencyMs(100);
			break;

		case ID_LATENCY_250MS:
			a->chip8->SetAudioLatencyMs(250);
			break;

		case ID_LATENCY_500MS:
			a->chip8->SetAudioLatencyMs(500);
			break;

		case ID_LATENCY_1000MS:
			a->chip8->SetAudioLatencyMs(1000);
			break;

		case ID_ZOOM_1X1:
			a->zoom = 1;
			a->reflectZoom();
			break;

		case ID_ZOOM_2X2:
			a->zoom = 2;
			a->reflectZoom();
			break;

		case ID_ZOOM_4X4:
			a->zoom = 4;
			a->reflectZoom();
			break;

		case ID_ZOOM_8X8:
			a->zoom = 8;
			a->reflectZoom();
			break;

		case ID_ZOOM_16X16:
			a->zoom = 16;
			a->reflectZoom();
			break;

		case ID_ZOOM_32X32:
			a->zoom = 32;
			a->reflectZoom();
			break;

		case ID_SPEED_1INSTRUCTION:
			a->chip8->SetSpeed(1);
			break;

		case ID_SPEED_2INSTRUCTIONS:
			a->chip8->SetSpeed(2);
			break;

		case ID_SPEED_5INSTRUCTIONS:
			a->chip8->SetSpeed(5);
			break;

		case ID_SPEED_10INSTRUCTIONS:
			a->chip8->SetSpeed(10);
			break;

		case ID_SPEED_20INSTRUCTIONS:
			a->chip8->SetSpeed(20);
			break;

		case ID_SPEED_50INSTRUCTIONS:
			a->chip8->SetSpeed(50);
			break;

		case ID_SPEED_100INSTRUCTIONS:
			a->chip8->SetSpeed(100);
			break;

		case ID_SPEED_200INSTRUCTIONS:
			a->chip8->SetSpeed(200);
			break;

		case ID_SPEED_500INSTRUCTIONS:
			a->chip8->SetSpeed(500);
			break;

		case ID_SPEED_1000INSTRUCTIONS:
			a->chip8->SetSpeed(1000);
			break;

		case ID_HELP_ABOUT:
			MessageBox(a->window->GetHandle(), "Chips8TheSalsa\n\nA Chip-8 emulator for Windows. What a piece of shit this system is :D . Ah well, it was fun as hell to code.\n\nThe key layout is a bit strange; it's a 16-key hex keyboard that's mapped like this:\n123C\n456D\n789E\nA0BF\n\nand corresponds to the following keys:\n1234\nQWER\nASDF\nZXCV\n\nHave fun :)\n\n(C) 2012 Jake \"Ferris\" Taylor\nhttp://yupferris.blogspot.com\nyupferris@gmail.com", "About Chips8TheSalsa", MB_OK);
			break;
		}

		a->reflectSettingsInMenu();
		break;

	case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				a->window->Close();
			}
			else
			{
				for (int i = 0; i < 16; i++)
				{
					if (wParam == keyChars[i])
					{
						a->chip8->SetInput(i, true);
						break;
					}
				}
			}
		}
		break;

	case WM_KEYUP:
		{
			for (int i = 0; i < 16; i++)
			{
				if (wParam == keyChars[i])
				{
					a->chip8->SetInput(i, false);
					break;
				}
			}
		}
		break;
	}
}

void Application::reflectZoom()
{
	window->SetSize(64 * zoom, 32 * zoom);
}

void Application::reflectSettingsInMenu()
{
	CheckMenuItem(menu, ID_AUDIO_ENABLED, chip8->GetAudioEnabled() ? MF_CHECKED : MF_UNCHECKED);
	int l = chip8->GetAudioLatencyMs();
	CheckMenuItem(menu, ID_LATENCY_50MS, (l == 50) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_LATENCY_100MS, (l == 100) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_LATENCY_250MS, (l == 250) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_LATENCY_500MS, (l == 500) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_LATENCY_1000MS, (l == 1000) ? MF_CHECKED : MF_UNCHECKED);

	CheckMenuItem(menu, ID_ZOOM_1X1, (zoom == 1) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_ZOOM_2X2, (zoom == 2) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_ZOOM_4X4, (zoom == 4) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_ZOOM_8X8, (zoom == 8) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_ZOOM_16X16, (zoom == 16) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_ZOOM_32X32, (zoom == 32) ? MF_CHECKED : MF_UNCHECKED);

	int s = chip8->GetSpeed();
	CheckMenuItem(menu, ID_SPEED_1INSTRUCTION, (s == 1) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_SPEED_2INSTRUCTIONS, (s == 2) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_SPEED_5INSTRUCTIONS, (s == 5) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_SPEED_10INSTRUCTIONS, (s == 10) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_SPEED_20INSTRUCTIONS, (s == 20) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_SPEED_50INSTRUCTIONS, (s == 50) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_SPEED_100INSTRUCTIONS, (s == 100) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_SPEED_200INSTRUCTIONS, (s == 200) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_SPEED_500INSTRUCTIONS, (s == 500) ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(menu, ID_SPEED_1000INSTRUCTIONS, (s == 1000) ? MF_CHECKED : MF_UNCHECKED);
}

void Application::loadAndStartRom(string fileName)
{
	chip8->Reset();
	chip8->LoadRom(fileName);
	chip8->Start();
}

void Application::setWorkingDirectory()
{
	char *cd = new char[MAX_PATH];
	for (int i = 0; i < MAX_PATH; i++) cd[i] = 0;
	GetModuleFileName(0, cd, MAX_PATH);
	for (int i = MAX_PATH - 1; i >= 0; i--)
	{
		if (cd[i] == '\\' || cd[i] == '/')
		{
			cd[i] = 0;
			break;
		}
	}
	SetCurrentDirectory(cd);
	delete [] cd;
}

typedef struct
{
	int AudioEnabled;
	int AudioLatencyMs;
	int Zoom;
	int Speed;
} settings;

void Application::loadSettings(string fileName)
{
	try
	{
		setWorkingDirectory();

		ifstream input(fileName, ios::binary | ios::ate);
		if (input.fail()) throw EXCEPTION("Could not open config file.");

		int size = (int) input.tellg();
		if (size != sizeof(settings)) throw EXCEPTION("Invalid settings format.");

		input.seekg(0);
		settings s;
		input.read((char *) &s, size);
		input.close();

		chip8->SetAudioEnabled(s.AudioEnabled != 0);
		chip8->SetAudioLatencyMs(s.AudioLatencyMs);
		zoom = s.Zoom;
		reflectZoom();
		chip8->SetSpeed(s.Speed);
	}
	catch (const Exception&) { }
}

void Application::saveSettings(string fileName)
{
	try
	{
		setWorkingDirectory();

		ofstream output(fileName, ios::binary);
		if (output.fail()) throw new EXCEPTION("Could not open config file.");

		settings s;
		s.AudioEnabled = chip8->GetAudioEnabled() ? 1 : 0;
		s.AudioLatencyMs = chip8->GetAudioLatencyMs();
		s.Zoom = zoom;
		s.Speed = chip8->GetSpeed();
		output.write((char *) &s, sizeof(settings));

		output.close();
	}
	catch (const Exception&) { }
}
