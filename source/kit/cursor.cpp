#include "cursor.h"
#include <SDL.h>

Vector2i Cursor::getPosition()
{
	return position;
}

bool Cursor::isPositionValid()
{
	return active && withinWindow;
}

bool Cursor::isActive()
{
	return active;
}

void Cursor::setActive(bool state)
{
	active = state;
	SDL_SetRelativeMouseMode(state ? SDL_FALSE : SDL_TRUE);
}

Cursor::Cursor()
{
	position.set(0, 0);
	withinWindow = true;
	active = true;
}

void Cursor::setPosition(Vector2i position)
{
	this->position = position;
}

void Cursor::setWithinWindow(bool state)
{
	withinWindow = state;
}

