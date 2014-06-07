#pragma once

#pragma comment (lib, "kit.lib")
#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")
#pragma comment (lib, "SDL2_image.lib")

#include <kit/app.h>
#include <kit/ptr.h>
#include <vector>
#include <string>

namespace kit
{
	void start (Ptr<App> app, std::vector<std::string> const & parameters);

	void finish ();
}

