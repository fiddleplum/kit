#include "audio.h"
#include <stdexcept>
#include <SDL.h>

Audio * instance = nullptr;

Audio::Audio(unsigned int channelConfig, unsigned int samplesPerSecond, unsigned int log2SamplesInBuffer, std::function<void(void *, unsigned int)> _callbackForMoreData)
{
	if(instance != nullptr)
	{
		throw std::runtime_error("There may be only one instance of audio at a time.");
	}
	instance = this;

	callbackForMoreData = _callbackForMoreData;

	SDL_InitSubSystem(SDL_INIT_AUDIO);

	SDL_AudioSpec desired;
	SDL_AudioSpec obtained;
	desired.freq = samplesPerSecond;
	desired.format = AUDIO_S16;
	switch(channelConfig)
	{
		case MONO: desired.channels = 1; break;
		case STEREO: desired.channels = 2; break;
		case QUAD: desired.channels = 4; break;
		case FIVE_POINT_ONE: desired.channels = 6; break;
		default: throw std::runtime_error("While initializing audio, an invalid channel configuration was specified.");
	}
	desired.silence = 0;
	desired.samples = 1 << log2SamplesInBuffer;
	desired.size = 0;
	desired.callback = callback;
	desired.userdata = 0;
	device = SDL_OpenAudioDevice(NULL, false, &desired, &obtained, 0);
	if(device == 0)
	{
		throw std::runtime_error("Failed to initialize audio.");
	}
}

Audio::~Audio()
{
	instance = nullptr;
	SDL_CloseAudioDevice(device);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Audio::setPaused(bool paused)
{
	SDL_PauseAudioDevice(device, paused ? 1 : 0);
}

void Audio::callback(void *, unsigned char * data, int size)
{
	instance->callbackForMoreData((void *)data, (unsigned int)size);
}

