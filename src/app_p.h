#pragma once

#include <kit/app.h>

namespace kit
{
	namespace app
	{
		// Initializes the application. Called by main.
		void initialize ();
		
		// Finalizes the application. Called by main.
		void finalize ();

		// Initiates and continues the application loop. Called by main.
		void loop ();
	}
}

