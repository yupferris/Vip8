#include "AudioStream.h"

AudioStream::AudioStream(RenderCallback renderCallback, void *userData, int latencyMs)
{
	this->latencyMs = latencyMs;
	this->renderCallback = renderCallback;
	this->userData = userData;

	init();
}

AudioStream::~AudioStream()
{
	free();
}

void AudioStream::Update()
{
	int playCursorPos;
	buffer->GetCurrentPosition((LPDWORD) &playCursorPos, 0);
	int bytesToRender = playCursorPos - oldPlayCursorPos;
	if (bytesToRender)
	{
		if (bytesToRender < 0) bytesToRender += bufferSizeBytes;
		AudioFormat::Sample *p1, *p2;
		int b1, b2;
		if (buffer->Lock(oldPlayCursorPos, bytesToRender, (LPVOID *) &p1, (LPDWORD) &b1, (LPVOID *) &p2, (LPDWORD) &b2, 0) != DS_OK) throw EXCEPTION("Could not lock buffer.");
		renderCallback(p1, b1 / sizeof(AudioFormat::Sample), userData);
		if (b2) renderCallback(p2, b2 / sizeof(AudioFormat::Sample), userData);
		if (buffer->Unlock(p1, b1, p2, b2) != DS_OK) throw EXCEPTION("Could not unlock buffer.");
		oldPlayCursorPos = playCursorPos;
	}
}

void AudioStream::SetLatencyMs(int latencyMs)
{
	if (this->latencyMs == latencyMs) return;
	free();
	this->latencyMs = latencyMs;
	init();
}

int AudioStream::GetLatencyMs() const
{
	return latencyMs;
}

void AudioStream::init()
{
	bufferSizeBytes = AudioFormat::SampleRate * AudioFormat::BlockAlign * latencyMs / 1000;

	if (DirectSoundCreate8(0, &device, 0) != DS_OK) throw EXCEPTION("Could not create device.");
	if (device->SetCooperativeLevel(GetForegroundWindow(), DSSCL_NORMAL) != DS_OK) throw EXCEPTION("Could not set device priority.");
	
	WAVEFORMATEX bufferFormat =
	{
		WAVE_FORMAT_PCM,
		AudioFormat::Channels,
		AudioFormat::SampleRate,
		AudioFormat::BytesPerSec,
		AudioFormat::BlockAlign,
		AudioFormat::BitsPerSample,
		0
	};
	DSBUFFERDESC bufferDesc =
	{
		sizeof(DSBUFFERDESC),
		DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2,
		bufferSizeBytes,
		0,
		&bufferFormat,
		GUID_NULL
	};
	if (device->CreateSoundBuffer(&bufferDesc, &buffer, 0) != DS_OK) throw EXCEPTION("Could not create secondary buffer.");

	oldPlayCursorPos = 0;
	if (buffer->Play(0, 0, DSBPLAY_LOOPING) != DS_OK) throw EXCEPTION("Could not start buffer playback.");
}

void AudioStream::free()
{
	buffer->Stop();
	buffer->Release();
	device->Release();
}
