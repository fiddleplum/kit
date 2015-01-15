#include "texture.h"
#include "open_gl.h"
#include <vector>
#include <SDL_image.h>

std::vector<unsigned int> currentTextures; // Current textures in the OpenGL state.

Texture::Texture(void const * pixels, Coord2i size_)
{
	size = size_;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size_[0], size_[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::Texture(std::string const & filename)
{
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
		default:
			throw std::runtime_error("Only 24 and 32 bits per pixel supported: " + filename);
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size[0], size[1], 0, format, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surface);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

Coord2i Texture::getSize() const
{
	return size;
}

void * Texture::getPixels() const
{
	// TODO
	return nullptr;
}

void Texture::setPixels(void const * pixels)
{
	// TODO
}

void Texture::activate(unsigned int slot) const
{
	if(slot >= currentTextures.size() || id != currentTextures[slot])
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
		if(slot >= currentTextures.size())
		{
			currentTextures.resize(slot + 1);
		}
		currentTextures[slot] = id;
	}
}

void Texture::deactivateRest(unsigned int slot)
{
	for(; slot < currentTextures.size(); slot++)
	{
		if(currentTextures[slot] == 0)
		{
			break; // If this one is zero, the rest are zero.
		}
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
		currentTextures[slot] = 0;
	}
}

