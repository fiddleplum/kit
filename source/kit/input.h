#pragma once

#include <string>

namespace input
{
	// Makes the cursor active or inactive. When it is inactive, the edges of the screen do not influence mouse movements.
	void setCursorActive(bool active);

	// Returns the number of controllers.
	int getNumControllers();

	// Gets the name of a controller.
	std::string getControllerName(int index);

	// Reloads the controllers, enabling plug-in and play functionality.
	void refreshControllers();

}

