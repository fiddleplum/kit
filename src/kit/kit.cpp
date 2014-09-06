#include "kit.h"
#include <iostream>
#include <SDL_main.h>
#include <SDL.h>

// Called by SDL to run the entire application.
int main(int argc, char *argv[])
{
#undef main // SDL redefines main, but it interferes with other things.

	int r = 0;

	try
	{
		// Grab the params.
		std::vector<std::string> args;
		for(int i = 1; i < argc; ++i) // don't include the 0th arg, because it is the program name
		{
			args.push_back(std::string(argv[i]));
		}

		// Call the user-supplied main.
		r = kit::main(args);
	}
	catch(std::exception const & e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return r;
}

namespace kit
{
	float getTime()
	{
		return SDL_GetTicks() / 1000.0f;
	}

	void wait(float seconds)
	{
		SDL_Delay(Uint32(seconds * 1000));
	}
}

