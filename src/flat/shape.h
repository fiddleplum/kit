#pragma once

#include "../vector.h"
#include <vector>

namespace Flat
{
	class Shape
	{
	public:
		enum { None, Circle, Rectangle, Polygon, NumTypes };

		int type;
	};

	class Circle : public Shape
	{
	public:
		Circle() { type = Shape::Circle; }

		Vector2f center;
		float radius;
	};

	class Rectangle : public Shape
	{
	public:
		Rectangle() { type = Shape::Rectangle; }

		float min;
		float max;
	};

	class Polygon : public Shape
	{
	public:
		Polygon() { type = Shape::Polygon; }

		std::vector<Vector2f> vertices;
	};
}

