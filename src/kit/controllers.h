#pragma once

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

		// Reloads the controllers, enabling plug-in and play functionality.
		void refresh();
	}
}

