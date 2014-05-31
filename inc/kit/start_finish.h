#pragma once

#pragma comment (lib, "kit.lib")
#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")
#pragma comment (lib, "SDL2_image.lib")

#include <vector>
#include <string>

namespace kit
{
	void start (std::vector<std::string> const & parameters);

	void finish ();
}

