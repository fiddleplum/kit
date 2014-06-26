#include "texture_p.h"
#include "open_gl.h"
#include "../external/SDL2_image-2.0.0/include/SDL_image.h"
#include <stdexcept>
#include <fstream>
#include <vector>

namespace kit
{
	std::vector<unsigned int> _currentTextures; // current textures in the open gl state

	TextureP::TextureP (Vector2i)
	{
		if(!glIsInitialized())
		{
			throw std::runtime_error("You must create a window first to initialize OpenGL.");
		}
		// TODO
	}

	TextureP::TextureP (std::string const & filename)
	{
		if(!glIsInitialized())
		{
			throw std::runtime_error("You must create a window first to initialize OpenGL.");
		}

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
			default:
				throw std::runtime_error("Only 24 and 32 bits per pixel supported: " + filename);
		}

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size[0], size[1], 0, format, GL_UNSIGNED_BYTE, surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	TextureP::~TextureP ()
	{
		glDeleteTextures (1, &id);
	}

	Vector2i TextureP::getSize () const
	{
		return size;
	}

	unsigned char * TextureP::lockPixels ()
	{
		// TODO
		return nullptr;
	}

	void TextureP::unlockPixels ()
	{
		// TODO
	}

	void TextureP::activate (unsigned int slot) const
	{
		if(slot >= _currentTextures.size() || id != _currentTextures[slot])
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, id);
			if(slot >= _currentTextures.size())
			{
				_currentTextures.resize(slot + 1);
			}
			_currentTextures[slot] = id;
		}
	}

	void TextureP::deactivateRest (unsigned int slot)
	{
		for(; slot < _currentTextures.size(); slot++)
		{
			if(_currentTextures[slot] == 0)
			{
				break; // If this one is zero, the rest are zero.
			}
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, 0);
			_currentTextures[slot] = 0;
		}
	}
}

