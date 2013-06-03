#include "Texture.h"
#include "ResourceManager.h"
#include <stdexcept>
#include <fstream>

#define cimg_OS 1
#define cimg_display 0
#define cimg_use_png
#include <stdio.h>
#include <CImg.h>

class Texture::Resource
{
public:
	Resource(std::string const & filename)
	{
		cimg_library::CImg<unsigned char> image {filename.c_str()};
	}

	char * data;
};

static ResourceManager<Texture::Resource> gTextureManager;

Texture::Texture(std::string const & filename)
{
	mResource = gTextureManager.get(filename, filename);
}

