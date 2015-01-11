#pragma once

#include <vector>
#include <string>

namespace kit
{
	// The user must implement this function.
	int main(std::vector<std::string> const & args);

	// Waits the given amount of time.
	void wait(float seconds);
}

