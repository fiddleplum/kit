#include "Texture.h"
#include "OpenGL.h"
#include "../../SDL2_image-2.0.0/include/SDL_image.h"
#include <stdexcept>
#include <fstream>
#include <vector>

std::vector<unsigned int> gCurrentTextures;

Texture::Texture(std::string const & filename)
{
	glGenTextures(1, &id);

	SDL_Surface * surface = IMG_Load(filename.c_str());
	if(surface == 0)
	{
		throw std::runtime_error("Could not load texture: " + filename);
	}
	size[0] = surface->w;
	size[1] = surface->h;

	GLenum format;
	switch(surface->format->BitsPerPixel)
	{
		case 24:
			format = GL_RGB; break;
		case 32:
			format = GL_RGBA; break;
	}

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size[0], size[1], 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

Vector2i Texture::getSize() const
{
	return size;
}

void Texture::activate(unsigned int slot) const
{
	if(slot >= gCurrentTextures.size() || id != gCurrentTextures[slot])
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
		if(slot >= gCurrentTextures.size())
		{
			gCurrentTextures.resize(slot + 1);
		}
		gCurrentTextures[slot] = id;
	}
}

void Texture::deactivateRest(unsigned int slot)
{
	for(; slot < gCurrentTextures.size(); slot++)
	{
		if(gCurrentTextures[slot] == 0)
		{
			break; // If this one is zero, the rest are zero.
		}
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
		gCurrentTextures[slot] = 0;
	}
}

