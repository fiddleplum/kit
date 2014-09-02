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
	int main(std::vector<std::string> const & args);
}

