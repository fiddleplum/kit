#pragma once

#ifdef _DEBUG
#pragma comment (lib, "kit-d.lib")
#else
#pragma comment (lib, "kit.lib")
#endif
#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")

#include <vector>
#include <string>

namespace kit
{
	// The user must implement this function.
	int main(std::vector<std::string> const & args);

	// Gets the time in seconds since some arbitrary time after the application started, up to 49 days.
	float getTime();

	// Waits the given amount of time.
	void wait(float seconds);
}

