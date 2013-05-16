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

		auto loadAndStartRomImage = [&] (const String& fileName)
			{
				try
				{
					chip8.LoadRom(File::ReadAllBytes(fileName));
					chip8.Start();
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
					chip8.Reset();
					loadAndStartRomImage(fileName);
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
		const int numLatencies = 5;
		const int latencies[] = { 50, 100, 250, 500, 1000 };
		int latencyIndex = 0;
		List<MenuItem *> systemAudioLatencyItems;
		auto reflectLatencyIndex = [&]
			{
				auto latency = latencies[latencyIndex];
				// TODO: make functional
				for (int i = 0; i < systemAudioLatencyItems.Count(); i++) systemAudioLatencyItems[i]->SetChecked(i == latencyIndex);
			};
		for (int i = 0; i < numLatencies; i++)
		{
			auto latency = latencies[i];
			auto item = MenuItem::Create(String(latency) + " ms");
			item->Click += [&, i]
				{
					latencyIndex = i;
					reflectLatencyIndex();
				};
			systemAudioLatencyMenu->AddChild(item);
			systemAudioLatencyItems.Add(item);
		}
		reflectLatencyIndex();
		systemAudioMenu->AddChild(systemAudioLatencyMenu);
		systemMenu->AddChild(systemAudioMenu);
		
		auto systemVideoMenu = Menu::Create("Video");
		auto systemVideoZoomMenu = Menu::Create("Zoom");
		const int numZooms = 4;
		const int zooms[] = { 4, 8, 16, 32 };
		int zoomIndex = 1;
		List<MenuItem *> systemVideoZoomItems;
		auto reflectZoomIndex = [&]
			{
				auto zoom = zooms[zoomIndex];
				window->SetDesiredSize(chip8.GetOutputWidth() * zoom, chip8.GetOutputHeight() * zoom);
				for (int i = 0; i < systemVideoZoomItems.Count(); i++) systemVideoZoomItems[i]->SetChecked(i == zoomIndex);
			};
		for (int i = 0; i < numZooms; i++)
		{
			auto zoom = zooms[i];
			auto item = MenuItem::Create(String(zoom) + "x" + zoom);
			item->Click += [&, i]
				{
					zoomIndex = i;
					reflectZoomIndex();
				};
			systemVideoZoomMenu->AddChild(item);
			systemVideoZoomItems.Add(item);
		}
		reflectZoomIndex();
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

		if (arguments.Count()) loadAndStartRomImage(arguments[0]); // TODO: proper arg's

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
		for (int i = 0; i < systemAudioLatencyItems.Count(); i++) delete systemAudioLatencyItems[i];

		delete systemVideoMenu;
		delete systemVideoZoomMenu;
		for (int i = 0; i < systemVideoZoomItems.Count(); i++) delete systemVideoZoomItems[i];

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
