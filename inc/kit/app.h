#pragma once

#include <kit/ptr.h>
#include <kit/vector.h>

namespace kit
{
	class Window;

	namespace app
	{
		// Quits the application.
		void quit ();

		// Adds a new window.
		Ptr<Window> addWindow (char const * title);

		// Removes a window.
		void removeWindow (Ptr<Window> window);

		// Gets the time in seconds since some arbitrary time after the application started, up to 49 days.
		float getTime ();

		// Makes the cursor active or inactive. When it is inactive, the edges of the screen do not influence mouse movements.
		void setCursorActive (bool active);
	}
}

