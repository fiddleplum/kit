#include "time.h"
#include <SDL.h>

float Time::getSecondsSinceStart()
{
	return SDL_GetTicks() / 1000.0f;
}

