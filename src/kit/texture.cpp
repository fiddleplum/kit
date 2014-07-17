#include "texture.h"
#include "open_gl.h"
#include <vector>
#include "../../external/SDL2_image/include/SDL_image.h"

namespace kit
{
	std::vector<unsigned int> _currentTextures; // current textures in the open gl state

	Texture::Texture(Vector2i)
	{
		if (!glIsInitialized())
		{
			throw std::runtime_error("You must create a window first to initialize OpenGL.");
		}
		// TODO
	}

	Texture::Texture(std::string const & filename)
	{
		if (!glIsInitialized())
		{
			throw std::runtime_error("You must create a window first to initialize OpenGL.");
		}

		SDL_Surface * surface = IMG_Load(filename.c_str());
		if (surface == 0)
		{
			throw std::runtime_error("Could not load texture: " + filename);
		}
		size[0] = surface->w;
		size[1] = surface->h;

		GLenum format;
		switch (surface->format->BitsPerPixel)
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
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}

	Vector2i Texture::getSize() const
	{
		return size;
	}

	unsigned char * Texture::lockPixels()
	{
		// TODO
		return nullptr;
	}

	void Texture::unlockPixels()
	{
		// TODO
	}

	void Texture::activate(unsigned int slot) const
	{
		if (slot >= _currentTextures.size() || id != _currentTextures[slot])
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, id);
			if (slot >= _currentTextures.size())
			{
				_currentTextures.resize(slot + 1);
			}
			_currentTextures[slot] = id;
		}
	}

	void Texture::deactivateRest(unsigned int slot)
	{
		for (; slot < _currentTextures.size(); slot++)
		{
			if (_currentTextures[slot] == 0)
			{
				break; // If this one is zero, the rest are zero.
			}
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, 0);
			_currentTextures[slot] = 0;
		}
	}
}

