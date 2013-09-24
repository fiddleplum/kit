#pragma once

#include "Vector.h"
#include <string>

class Texture
{
public:
	Texture(std::string const & filename);

	~Texture();

	Vector2i getSize() const;

	void activate(unsigned int slot) const;

	static void deactivateRest(unsigned int slot); // Deactivates all slots equal to or greater than slot.

private:
	Vector2i size;
	unsigned int id;
};

