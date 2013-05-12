#ifndef __AUDIO_FORMAT_H__
#define __AUDIO_FORMAT_H__

class AudioFormat
{
public:
	static const int SampleRate = 44100;
	static const int Channels = 2;
	static const int BitsPerSample = 16;
	static const int BlockAlign = Channels * BitsPerSample / 8;
	static const int BytesPerSec = SampleRate * BlockAlign;

	typedef short Sample;
};

#endif
