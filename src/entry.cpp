#include <kit/entry.h>
#include "../external/SDL2-2.0.0/include/SDL.h"

// Called by SDL to start things off.
int main (int argc, char *argv[])
{
	try
	{
		// Grab the params.
		std::vector<std::string> parameters;
		for(int i = 1; i < argc; ++i)
		{
			parameters.push_back(std::string(argv[i]));
		}

		// Start SDL.
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) == -1)
		{
			throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
		}

		kit::entry(parameters);

		// Finish SDL.
		SDL_Quit();
	}
	catch(std::runtime_error const & e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
		return -1;
	}

	return 0;
}

