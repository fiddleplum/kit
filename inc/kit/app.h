#pragma once

#include <kit/ptr.h>
#include <kit/vector.h>

namespace kit
{
	class Window;

	class App
	{
	public:
		// Quits the application.
		virtual void quit () = 0;

		// Adds a new window.
		virtual Ptr<Window> addWindow (char const * title) = 0;

		// Removes a window.
		virtual void removeWindow (Ptr<Window> window) = 0;

		// Gets the time in seconds since some arbitrary time after the application started, up to 49 days.
		virtual float getTime () const = 0;

		// Returns the cursor position. If the cursor does not have a valid position, it fails an assertion.
		virtual Vector2i getCursorPosition () const = 0;
	};
}

