#pragma once

#include "coord.h"
#include <string>

#pragma comment (lib, "SDL2_image.lib")

class Texture
{
public:
	// Creates a new texture from raw RGBA32 pixels.
	Texture(void const * pixels, Coord2i size);

	// Creates a texture from a file.
	Texture(std::string const & filename);

	// Destroys the texture.
	virtual ~Texture();

	// Returns the size of the texture.
	Coord2i getSize() const;

	// Returns the raw RGBA32 pixels of the texture.
	void * getPixels() const;

	// Sets the texture content from raw RGBA32 pixels.
	void setPixels(void const * pixels);

	// Activates the texture in the GL slot. Used by Models.
	void activate(unsigned int slot) const;

	// Deactivates all slots equal to or greater than the GL slot. Used by Models.
	static void deactivateRest(unsigned int slot);

private:
	Coord2i size;
	unsigned int id;
};

