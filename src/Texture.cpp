#include "Texture.h"
#include "OpenGL.h"
#include <SDL_Image.h>
#include <stdexcept>
#include <fstream>
#include <vector>

std::vector<unsigned int> gCurrentTextures;

Texture::Texture(std::string const & filename)
{
	glGenTextures(1, &mHandle);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mHandle);
}

void Texture::activate(unsigned int slot) const
{
	if(slot >= gCurrentTextures.size() || mHandle != gCurrentTextures[slot])
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mHandle);
		if(slot >= gCurrentTextures.size())
		{
			gCurrentTextures.resize(slot + 1);
		}
		gCurrentTextures[slot] = mHandle;
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

