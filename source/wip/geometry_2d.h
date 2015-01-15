#pragma once

#include "geometry_1d.h"
#include "rect.h"
#include <vector>

namespace kit
{
	class Circle
	{
	public:
		Coord2f center;
		float radius;
	};

	class Polygon
	{
	public:
		std::vector<Coord2f> vertices;

		Rectf getBoundingRect() const;
	};
}

