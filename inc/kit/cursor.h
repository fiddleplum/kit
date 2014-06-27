#pragma once

#include <kit/vector.h>

namespace kit
{
	class Cursor
	{
	public:
		// Returns true if the cursor position is valid.
		virtual bool isValid () const = 0;

		// Gets the cursor position relative to the window.
		virtual Vector2i getPosition () const = 0;
	};
}

