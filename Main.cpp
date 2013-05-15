#include "Common.h"
#include "Chip8/Chip8.h"
#include "GLVideoModule.h"

int Main(const List<String>& arguments)
{
	try
	{
		Chip8 chip8;
		int zoom = 8;

		auto running = true;

		auto window = Window::Create("Vip8");
		window->KeyDown += [&] (Key key) { if (key == Key::Escape) running = false; };
		window->Closing += [&] () { running = false; };

		auto resizeWindow = [&] () { window->SetDesiredSize(chip8.GetOutputWidth() * zoom, chip8.GetOutputHeight() * zoom); };
		resizeWindow();

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
		fileLoadRomImage->Click += [&] ()
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
		fileExit->Click += [&] () { running = false; };
		fileMenu->AddChild(fileExit);
		menu->AddChild(fileMenu);
		auto systemMenu = Menu::Create("System");
		auto systemReset = MenuItem::Create("Reset");
		systemReset->Click += [&] ()
			{
				chip8.Reset();
				if (chip8.HasRom()) chip8.Start();
			};
		systemMenu->AddChild(systemReset);
		systemMenu->AddSeparator();
		auto systemAudioMenu = Menu::Create("Audio");
		auto systemAudioEnabled = MenuItem::Create("Enabled");
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
		// TODO: finish system menu
		menu->AddChild(systemMenu);
		auto helpMenu = Menu::Create("Help");
		auto helpAbout = MenuItem::Create("About...");
		helpAbout->Click += [&] () { MessageWindow::Info(window, "Vip8 - A Chip-8 emulator"); };
		helpMenu->AddChild(helpAbout);
		menu->AddChild(helpMenu);
		window->SetMenu(menu);

		auto viewport = Viewport::Create();
		window->SetContent(viewport);

		auto videoModule = new GLVideoModule(viewport);
		chip8.SetVideoModule(videoModule);

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
		// merhp
		delete helpMenu;
		delete helpAbout;
		delete viewport;
		delete videoModule;
	}
	catch (const Exception& e)
	{
		MessageWindow::Error("ERROR: " + e.GetMsg());
		return 1;
	}

	return 0;
}
