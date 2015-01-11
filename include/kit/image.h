#pragma once

#include "vector.h"
#include <vector>

class Image
{
public:
	class Color
	{
	public:
		float r, g, b, a;
	};

	inline Image(Vector2i size);

	inline Vector2i getSize() const;

	inline Color const & getPixel(Vector2i coord) const;
	
	inline Color & getPixel(Vector2i coord);

	inline Color const * getRawPixels() const;

private:
	std::vector<Color> pixels;
	Vector2i size;
};

// Implementations

Image::Image(Vector2i size_)
{
	size = size_;
	pixels.resize(size_[0] * size_[1]);
}

Vector2i Image::getSize() const
{
	return size;
}

Image::Color const & Image::getPixel(Vector2i coord) const
{
	return pixels[coord[1] * size[0] + coord[0]];
}

Image::Color & Image::getPixel(Vector2i coord)
{
	return pixels[coord[1] * size[0] + coord[0]];
}

Image::Color const * Image::getRawPixels() const
{
	return &pixels[0];
}

