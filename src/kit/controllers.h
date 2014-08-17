#pragma once

#include <string>

namespace kit
{
	namespace controllers
	{
		// Initialize the controller system
		void initialize();

		// Finalize the controller system
		void finalize();

		// Returns the number of controllers.
		int getNumControllers();

		// Gets the name of a controller.
		std::string getName(int controller);

		// Reloads the controllers, enabling plug-in and play functionality.
		void refresh();
	}
}

