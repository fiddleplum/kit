#pragma once

#include "window.h"
#include "scene.h"

namespace kit
{
	namespace app
	{
		// Quits the application.
		void quit ();

		// Adds a new window.
		Ptr<Window> addWindow (char const * title);

		// Removes a window.
		void removeWindow (Ptr<Window> window);

		// Adds a scene.
		Ptr<Scene> addScene ();

		// Removes a scene.
		void removeScene (Ptr<Scene> scene);

		// Gets the time in seconds since some arbitrary time after the application started, up to 49 days.
		float getTime ();

		// Makes the cursor active or inactive. When it is inactive, the edges of the screen do not influence mouse movements.
		void setCursorActive (bool active);

		// Initializes the application. Called by main.
		void initialize ();
		
		// Finalizes the application. Called by main.
		void finalize ();

		// Initiates and continues the application loop. Called by main.
		void loop ();
	}
}

