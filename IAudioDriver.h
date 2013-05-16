#ifndef __IAUDIODRIVER_H__
#define __IAUDIODRIVER_H__

class IAudioDriver
{
public:
	typedef void (* RenderCallback)(float *leftBuffer, float *rightBuffer, int numSamples, void *userData);

	virtual ~IAudioDriver();

	virtual void SetLatencyMs(int latencyMs) = 0;
	virtual int GetLatencyMs() const = 0;

	virtual int GetSampleRate() const = 0;
};

#endif
