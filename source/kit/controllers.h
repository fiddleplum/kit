#pragma once

#include <string>
#include <vector>

namespace kit
{
	namespace controllers
	{
		// Initialize the controller system
		void initialize();

		// Finalize the controller system
		void finalize();

		void startFrame();

		void updateControllerAxis(int controllerIndex, int axis, float value);

		std::vector<std::pair<int, float>> getAxesChangedSinceLastFrame(int controllerIndex);
	}
}

