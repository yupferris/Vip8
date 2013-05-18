#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Common.h"

class Config
{
public:
	Config(const String& fileName);
	
	void Save();

	bool AudioEnabled;
	int AudioLatencyIndex;
	int VideoZoomIndex;
	int SystemSpeedIndex;

private:
	static bool tryGetValue(const List<String>& input, const String& name, int& value);

	String fileName;
};

#endif
