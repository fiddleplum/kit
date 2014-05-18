#pragma once

#include "geometry_1d.h"
#include <vector>

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

