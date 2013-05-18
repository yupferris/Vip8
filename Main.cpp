#include "Common.h"
#include "Chip8/Chip8.h"
#include "Config.h"
#include "GLVideoDriver.h"
#include "DirectSoundAudioDriver.h"

int Main(const List<String>& arguments)
{
	try
	{
		Chip8 chip8;

		auto videoDriver = new GLVideoDriver();
		chip8.SetVideoDriver(videoDriver);

		auto audioDriver = new DirectSoundAudioDriver();
		chip8.SetAudioDriver(audioDriver);

		const int numLatencies = 8;
		const int latencies[] = { 20, 40, 60, 80, 100, 200, 500, 1000 };

		const int numZooms = 4;
		const int zooms[] = { 4, 8, 16, 32 };

		const int numSpeeds = 10;
		const int speeds[] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000 };

		Config config(Directory::GetApplicationDirectory() + "\\config.cfg");

		auto running = true;

		auto window = Window::Create("Vip8");
			auto menu = Menu::Create();
				auto fileMenu = Menu::Create("File");
					auto fileLoadRomImage = MenuItem::Create("Load ROM Image...");
					fileMenu->AddChild(fileLoadRomImage);
					fileMenu->AddSeparator();
					auto fileExit = MenuItem::Create("Exit");
					fileMenu->AddChild(fileExit);
				menu->AddChild(fileMenu);
				auto systemMenu = Menu::Create("System");
					auto systemReset = MenuItem::Create("Reset");
					systemMenu->AddChild(systemReset);
					systemMenu->AddSeparator();
					auto systemAudioMenu = Menu::Create("Audio");
						auto systemAudioEnabled = MenuItem::Create("Enabled");
						systemAudioEnabled->SetToggleEnabled(true);
						systemAudioMenu->AddChild(systemAudioEnabled);
						auto systemAudioLatencyMenu = Menu::Create("Latency");
							List<MenuItem *> systemAudioLatencyItems;
							for (int i = 0; i < numLatencies; i++)
							{
								auto latency = latencies[i];
								auto item = MenuItem::Create(String(latency) + " ms");
								systemAudioLatencyMenu->AddChild(item);
								systemAudioLatencyItems.Add(item);
							}
						systemAudioMenu->AddChild(systemAudioLatencyMenu);
					systemMenu->AddChild(systemAudioMenu);
					auto systemVideoMenu = Menu::Create("Video");
						auto systemVideoZoomMenu = Menu::Create("Zoom");
							List<MenuItem *> systemVideoZoomItems;
							for (int i = 0; i < numZooms; i++)
							{
								auto zoom = zooms[i];
								auto item = MenuItem::Create(String(zoom) + "x" + zoom);
								systemVideoZoomMenu->AddChild(item);
								systemVideoZoomItems.Add(item);
							}
						systemVideoMenu->AddChild(systemVideoZoomMenu);
					systemMenu->AddChild(systemVideoMenu);
					auto systemSpeedMenu = Menu::Create("Speed");
						List<MenuItem *> systemSpeedItems;
						for (int i = 0; i < numSpeeds; i++)
						{
							auto speed = speeds[i];
							auto item = MenuItem::Create(String(speed) + " instruction" + (speed > 1 ? "s" : "") + "/frame");
							systemSpeedMenu->AddChild(item);
							systemSpeedItems.Add(item);
						}
					systemMenu->AddChild(systemSpeedMenu);
				menu->AddChild(systemMenu);
				auto helpMenu = Menu::Create("Help");
					auto helpAbout = MenuItem::Create("About...");
					helpMenu->AddChild(helpAbout);
				menu->AddChild(helpMenu);
			window->SetMenu(menu);
			auto viewport = Viewport::Create();
			window->SetContent(viewport);

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

		auto reflectAudioEnabled = [&]
			{
				config.AudioEnabled = systemAudioEnabled->GetChecked();
				audioDriver->SetEnabled(config.AudioEnabled);
			};

		auto reflectLatencyIndex = [&]
			{
				auto latency = latencies[config.AudioLatencyIndex];
				audioDriver->SetLatencyMs(latency);
				for (int i = 0; i < systemAudioLatencyItems.Count(); i++) systemAudioLatencyItems[i]->SetChecked(i == config.AudioLatencyIndex);
			};

		auto reflectZoomIndex = [&]
			{
				auto zoom = zooms[config.VideoZoomIndex];
				window->SetDesiredSize(chip8.GetOutputWidth() * zoom, chip8.GetOutputHeight() * zoom);
				for (int i = 0; i < systemVideoZoomItems.Count(); i++) systemVideoZoomItems[i]->SetChecked(i == config.VideoZoomIndex);
			};

		auto reflectSpeedIndex = [&]
			{
				auto speed = speeds[config.SystemSpeedIndex];
				chip8.SetSpeed(speed);
				for (int i = 0; i < systemSpeedItems.Count(); i++) systemSpeedItems[i]->SetChecked(i == config.SystemSpeedIndex);
			};

		const Key keys[] = { Key::X, Key::Number1, Key::Number2, Key::Number3, Key::Q, Key::W, Key::E, Key::A, Key::S, Key::D, Key::Z, Key::C, Key::Number4, Key::R, Key::F, Key::V };
		window->KeyDown += [&] (Key key)
			{
				if (key == Key::Escape)
				{
					running = false;
				}
				else
				{
					for (int i = 0; i < 16; i++)
					{
						if (key == keys[i])
						{
							chip8.SetInput(i, true);
							break;
						}
					}
				}
			};
		window->KeyUp += [&] (Key key)
			{
				for (int i = 0; i < 16; i++)
				{
					if (key == keys[i])
					{
						chip8.SetInput(i, false);
						break;
					}
				}
			};

		window->Closing += [&] { running = false; };

		fileLoadRomImage->Click += [&]
			{
				auto fileName = DialogWindow::OpenFile(window, "Load ROM Image");
				if (fileName.Length())
				{
					chip8.Reset();
					loadAndStartRomImage(fileName);
				}
			};

		fileExit->Click += [&] { running = false; };

		systemReset->Click += [&]
			{
				chip8.Reset();
				if (chip8.HasRom()) chip8.Start();
			};

		systemAudioEnabled->CheckedChanged += reflectAudioEnabled;

		for (int i = 0; i < systemAudioLatencyItems.Count(); i++)
		{
			auto item = systemAudioLatencyItems[i];
			item->Click += [&, i]
				{
					config.AudioLatencyIndex = i;
					reflectLatencyIndex();
				};
		}

		for (int i = 0; i < systemVideoZoomItems.Count(); i++)
		{
			auto item = systemVideoZoomItems[i];
			item->Click += [&, i]
				{
					config.VideoZoomIndex = i;
					reflectZoomIndex();
				};
		}

		for (int i = 0; i < systemSpeedItems.Count(); i++)
		{
			auto item = systemSpeedItems[i];
			item->Click += [&, i]
				{
					config.SystemSpeedIndex = i;
					reflectSpeedIndex();
				};
		}

		helpAbout->Click += [&] { MessageWindow::Info(window, "Vip8 - A Chip-8 emulator"); };

		videoDriver->SetViewport(viewport);

		systemAudioEnabled->SetChecked(config.AudioEnabled);
		reflectAudioEnabled();
		reflectLatencyIndex();
		reflectZoomIndex();
		reflectSpeedIndex();
		if (arguments.Count() == 1) loadAndStartRomImage(arguments[0]);

		while (running)
		{
			try
			{
				chip8.Update();
			}
			catch (const Exception& e)
			{
				MessageWindow::Error(window, "Emulation error: " + e.GetMsg());
			}
			window->Update();
		}

		config.Save();

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
				delete systemSpeedMenu;
					for (int i = 0; i < systemSpeedItems.Count(); i++) delete systemSpeedItems[i];
				delete helpMenu;
					delete helpAbout;
			delete viewport;

		delete videoDriver;
		delete audioDriver;
	}
	catch (const Exception& e)
	{
		MessageWindow::Error("ERROR: " + e.GetMsg());
		return 1;
	}

	return 0;
}
