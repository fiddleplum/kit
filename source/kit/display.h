#pragma once

#include "coord.h"
#include <vector>

namespace Display
{
	int getNumDisplays();

	Coord2i getStartingResolution(int display);

	Coord2i getCurrentResolution(int display);

	std::vector<Coord2i> getAllResolutions(int display);
}

