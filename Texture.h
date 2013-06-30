#pragma once

#include "ResourceManager.h"
#include <string>

class Texture
{
public:
	Texture(std::string const & filename);

	~Texture();

	void activate(unsigned int slot);
	
	static void deactivate(unsigned int slot);

private:
	unsigned int mHandle;
};

extern ResourceManager<Texture, std::string> * gTextureManager;

