#pragma once

#include <string>
#include <vector>

class ControllerSystem
{
public:
	// Initializes the controller system.
	static void initialize();

	// Finalizes the controller system.
	static void finalize();

	// Returns the number of controllers plugged in.
	static int getNumControllers();

	// Returns the name of a given controller.
	static std::string getName(int controller);

	void startFrame();

	std::vector<std::pair<int, float>> getAxesChangedSinceLastFrame(int controllerIndex);
};

