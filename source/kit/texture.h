#pragma once

#include "vector.h"
#include <string>

class Texture
{
public:
	// Creates a new texture. The pixels are uninitialized.
	// Size is the size of the new texture.
	Texture(Vector2i size);

	// Creates a texture from the filesystem.
	// Filename is the name of the file to load.
	Texture(std::string const & filename);

	// Destroys the texture.
	~Texture();

	// Returns the size of the texture.
	Vector2i getSize() const;

	// Returns the raw pixels of the texture (RGBA 32-bit). While locked, the texture is not rendered.
	unsigned char * lockPixels();

	// Unlocks the pixels of the texture, after a call to lockPixels.
	void unlockPixels();

	// Activates the texture in the GL slot. Used by Models.
	void activate(unsigned int slot) const;

	// Deactivates all slots equal to or greater than the GL slot. Used by Models.
	static void deactivateRest(unsigned int slot);

private:
	Vector2i size;
	unsigned int id;
};

