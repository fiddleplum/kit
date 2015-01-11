#pragma once

#include <kit/vector.h>
#include <string>

class Image;

class Texture
{
public:
	// Creates a new texture from an image.
	Texture(Image const & image);

	// Creates a texture from a file.
	Texture(std::string const & filename);

	// Destroys the texture.
	virtual ~Texture();

	// Returns the size of the texture.
	Vector2i getSize() const;

	// Returns the image of the texture.
	Image getImage();

	// Sets the texture to the image.
	void setImage(Image const & image);

	// Activates the texture in the GL slot. Used by Models.
	void activate(unsigned int slot) const;

	// Deactivates all slots equal to or greater than the GL slot. Used by Models.
	static void deactivateRest(unsigned int slot);

private:
	Vector2i size;
	unsigned int id;
};

