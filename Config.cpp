#include "Config.h"

Config::Config(const String& fileName)
{
	this->fileName = fileName;
	AudioEnabled = true;
	AudioLatencyIndex = 2;
	VideoZoomIndex = 1;
	SystemSpeedIndex = 4;

	try
	{
		auto input = File::ReadAllLines(fileName);
		
		int value;
		if (tryGetValue(input, "AudioEnabled", value)) AudioEnabled = value != 0;
		if (tryGetValue(input, "AudioLatencyIndex", value)) AudioLatencyIndex = value;
		if (tryGetValue(input, "VideoZoomIndex", value)) VideoZoomIndex = value;
		if (tryGetValue(input, "SystemSpeedIndex", value)) SystemSpeedIndex = value;
	}
	catch (...) { }
}

void Config::Save()
{
	String output;

	output += String("AudioEnabled:") + AudioEnabled + "\n";
	output += String("AudioLatencyIndex:") + AudioLatencyIndex + "\n";
	output += String("VideoZoomIndex:") + VideoZoomIndex + "\n";
	output += String("SystemSpeedIndex:") + SystemSpeedIndex + "\n";

	try
	{
		File::WriteAllText(fileName, output);
	}
	catch (...) { }
}

bool Config::tryGetValue(const List<String>& input, const String& name, int& value)
{
	for (int i = 0; i < input.Count(); i++)
	{
		auto parts = input[i].Split(':');
		if (parts.Count() == 2 && parts[0] == name)
		{
			if (parts[1].TryParseInt(value)) return true;
		}
	}
	return false;
}
