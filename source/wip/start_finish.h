#pragma once

#ifdef _DEBUG
	#pragma comment (lib, "kit-d.lib")
#else
	#pragma comment (lib, "kit.lib")
#endif
#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")
#pragma comment (lib, "SDL2_image.lib")
#pragma comment (lib, "SDL2_mixer.lib")

#include "app.h"
#include <vector>
#include <string>

namespace kit
{
	//void start (std::vector<std::string> const & parameters);

	//void finish ();
}

