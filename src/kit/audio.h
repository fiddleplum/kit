#pragma once

#include <functional>

class Audio
{
public:
	enum ChannelConfig
	{
		MONO, STEREO, QUAD, FIVE_POINT_ONE
	};

	// Initialize the audio system.
	// The callback arguments are a buffer, and byte-size of that buffer. The buffer must be filled every time.
	// Each sample is a 16-bit signed integer.
	// The audio channels are interleaved, so the samples go like L, R, L, R... or FL, FR, RL, RR... or FL, FR, C, SUB, RL, RR...
	Audio(unsigned int channelConfig, unsigned int samplesPerSecond, unsigned int log2SamplesInBuffer, std::function<void (void *, unsigned int)> callbackForMoreData);

	// Finalize the audio system.
	~Audio();

	// This will stop or start the callback.
	void setPaused(bool paused);

private:
	static void callback(void *, unsigned char *, int);
	unsigned int device;
	std::function<void(void *, int)> callbackForMoreData;
};

