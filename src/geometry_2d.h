#pragma once

#include "geometry_1d.h"
#include <kit/rect.h>
#include <vector>

namespace kit
{
	class Circle
	{
	public:
		Vector2f center;
		float radius;
	};

	class Polygon
	{
	public:
		std::vector<Vector2f> vertices;

		Rectf getBoundingRect() const;
	};
}

