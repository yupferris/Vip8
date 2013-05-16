#include "Common.h"
#include "Chip8/Chip8.h"
#include "GLVideoDriver.h"

int Main(const List<String>& arguments)
{
	try
	{
		Chip8 chip8;

		auto running = true;

		auto window = Window::Create("Vip8");
		window->KeyDown += [&] (Key key) { if (key == Key::Escape) running = false; };
		window->Closing += [&] { running = false; };

		auto loadRomImage = [&] (const String& fileName)
			{
				try
				{
					chip8.LoadRom(File::ReadAllBytes(fileName));
				}
				catch (const Exception& e)
				{
					MessageWindow::Error(e.GetMsg());
				}
			};

		auto menu = Menu::Create();
		auto fileMenu = Menu::Create("File");
		auto fileLoadRomImage = MenuItem::Create("Load ROM Image...");
		fileLoadRomImage->Click += [&]
			{
				// TODO: File filters
				auto fileName = DialogWindow::OpenFile(window, "Load ROM Image");
				if (fileName.Length())
				{
					if (chip8.IsRunning()) chip8.Reset();
					loadRomImage(fileName);
					if (chip8.HasRom()) chip8.Start();
				}
			};
		fileMenu->AddChild(fileLoadRomImage);
		fileMenu->AddSeparator();
		auto fileExit = MenuItem::Create("Exit");
		fileExit->Click += [&] { running = false; };
		fileMenu->AddChild(fileExit);
		menu->AddChild(fileMenu);
		auto systemMenu = Menu::Create("System");
		auto systemReset = MenuItem::Create("Reset");
		systemReset->Click += [&]
			{
				chip8.Reset();
				if (chip8.HasRom()) chip8.Start();
			};
		systemMenu->AddChild(systemReset);
		systemMenu->AddSeparator();
		auto systemAudioMenu = Menu::Create("Audio");
		auto systemAudioEnabled = MenuItem::Create("Enabled");
		systemAudioEnabled->SetChecked(true);
		systemAudioEnabled->SetToggleEnabled(true);
		systemAudioMenu->AddChild(systemAudioEnabled);
		auto systemAudioLatencyMenu = Menu::Create("Latency");
		auto systemAudioLatency50ms = MenuItem::Create("50 ms");
		systemAudioLatencyMenu->AddChild(systemAudioLatency50ms);
		auto systemAudioLatency100ms = MenuItem::Create("100 ms");
		systemAudioLatencyMenu->AddChild(systemAudioLatency100ms);
		auto systemAudioLatency250ms = MenuItem::Create("250 ms");
		systemAudioLatencyMenu->AddChild(systemAudioLatency250ms);
		auto systemAudioLatency500ms = MenuItem::Create("500 ms");
		systemAudioLatencyMenu->AddChild(systemAudioLatency500ms);
		auto systemAudioLatency1000ms = MenuItem::Create("1000 ms");
		systemAudioLatencyMenu->AddChild(systemAudioLatency1000ms);
		systemAudioMenu->AddChild(systemAudioLatencyMenu);
		systemMenu->AddChild(systemAudioMenu);
		
		auto systemVideoMenu = Menu::Create("Video");
		auto systemVideoZoomMenu = Menu::Create("Zoom");
		const int numZooms = 4;
		const int zoomSizes[] = { 4, 8, 16, 32 };
		int zoomIndex = 1;
		auto systemVideoZoomItems = new MenuItem *[numZooms];
		auto reflectZoom = [&]
			{
				auto size = zoomSizes[zoomIndex];
				window->SetDesiredSize(chip8.GetOutputWidth() * size, chip8.GetOutputHeight() * size);
				for (int i = 0; i < numZooms; i++) systemVideoZoomItems[i]->SetChecked(i == zoomIndex);
			};
		for (int i = 0; i < numZooms; i++)
		{
			auto size = zoomSizes[i];
			systemVideoZoomItems[i] = MenuItem::Create(String(size) + "x" + size);
			systemVideoZoomItems[i]->Click += [&, i]
				{
					zoomIndex = i;
					reflectZoom();
				};
			systemVideoZoomMenu->AddChild(systemVideoZoomItems[i]);
		}
		reflectZoom();
		systemVideoMenu->AddChild(systemVideoZoomMenu);
		systemMenu->AddChild(systemVideoMenu);

		menu->AddChild(systemMenu);
		auto helpMenu = Menu::Create("Help");
		auto helpAbout = MenuItem::Create("About...");
		helpAbout->Click += [&] { MessageWindow::Info(window, "Vip8 - A Chip-8 emulator"); };
		helpMenu->AddChild(helpAbout);
		menu->AddChild(helpMenu);
		window->SetMenu(menu);

		auto viewport = Viewport::Create();
		window->SetContent(viewport);

		auto videoDriver = new GLVideoDriver(viewport);
		chip8.SetVideoDriver(videoDriver);

		if (arguments.Count())
		{
			loadRomImage(arguments[0]); // TODO: proper arg's
			if (chip8.HasRom()) chip8.Start();
		}

		while (running)
		{
			chip8.Update();
			window->Update();
		}

		delete window;
		delete menu;
		delete fileMenu;
		delete fileLoadRomImage;
		delete fileExit;
		delete systemMenu;
		delete systemReset;
		delete systemAudioMenu;
		delete systemAudioEnabled;
		delete systemAudioLatencyMenu;
		delete systemAudioLatency50ms;
		delete systemAudioLatency100ms;
		delete systemAudioLatency250ms;
		delete systemAudioLatency500ms;
		delete systemAudioLatency1000ms;

		delete systemVideoMenu;
		delete systemVideoZoomMenu;
		for (int i = 0; i < numZooms; i++) delete systemVideoZoomItems[i];
		delete [] systemVideoZoomItems;

		delete helpMenu;
		delete helpAbout;
		delete viewport;
		delete videoDriver;
	}
	catch (const Exception& e)
	{
		MessageWindow::Error("ERROR: " + e.GetMsg());
		return 1;
	}

	return 0;
}
