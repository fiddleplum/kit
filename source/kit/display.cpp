#include "display.h"
#include <string>
#include <SDL.h>

namespace Display
{
	int getNumDisplays()
	{
		int numDisplays = SDL_GetNumVideoDisplays();
		if(numDisplays < 0)
		{
			numDisplays = 0;
		}
		return numDisplays;
	}

	Coord2i getStartingResolution(int display)
	{
		SDL_DisplayMode mode;
		if(SDL_GetDesktopDisplayMode(display, &mode) == 0)
		{
			return{mode.w, mode.h};
		}
		throw std::runtime_error("Could not get the initial resolution of display " + std::to_string(display) + ". ");
	}

	Coord2i getCurrentResolution(int display)
	{
		SDL_DisplayMode mode;
		if(SDL_GetCurrentDisplayMode(display, &mode) == 0)
		{
			return{mode.w, mode.h};
		}
		throw std::runtime_error("Could not get the current resolution of display " + std::to_string(display) + ". ");
	}

	std::vector<Coord2i> getAllResolutions(int display)
	{
		std::vector<Coord2i> resolutions;

		SDL_DisplayMode desktopMode;
		if(SDL_GetDesktopDisplayMode(display, &desktopMode) == 0)
		{
			resolutions.push_back({desktopMode.w, desktopMode.h});
		}

		int numModes = SDL_GetNumDisplayModes(display);
		if(numModes < 0)
		{
			numModes = 0;
		}

		SDL_DisplayMode mode;
		for(int i = 0; i < numModes; i++)
		{
			if(SDL_GetDisplayMode(display, i, &mode) == 0 && mode.format == desktopMode.format && mode.refresh_rate == desktopMode.refresh_rate)
			{
				Coord2i resolution{mode.w, mode.h};
				for(auto it = resolutions.begin(); it != resolutions.end(); it++)
				{
					if(*it == resolution)
					{
						break;
					}
				}
				if(std::find(resolutions.begin(), resolutions.end(), resolution) == resolutions.end())
				{
					resolutions.push_back(resolution);
				}
			}
		}
		return resolutions;
	}
}

