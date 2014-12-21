#include "image.h"
#include <SDL_image.h>

int sdlImageInitializedCount = 0;

SDL_PixelFormat createPixelFormat(bool useAlpha)
{
	SDL_PixelFormat pixelFormat;
	pixelFormat.palette = NULL;
	if(useAlpha)
	{
		pixelFormat.format = SDL_PIXELFORMAT_RGBA8888;
		pixelFormat.BitsPerPixel = 32;
		pixelFormat.BytesPerPixel = 4;
		pixelFormat.Rmask = 0xff000000;
		pixelFormat.Gmask = 0x00ff0000;
		pixelFormat.Bmask = 0x0000ff00;
		pixelFormat.Amask = 0x000000ff;
	}
	else
	{
		pixelFormat.format = SDL_PIXELFORMAT_RGB24;
		pixelFormat.BitsPerPixel = 24;
		pixelFormat.BytesPerPixel = 3;
		pixelFormat.Rmask = 0xff0000;
		pixelFormat.Gmask = 0x00ff00;
		pixelFormat.Bmask = 0x0000ff;
	}
	return pixelFormat;
}

Image::Image(int width, int height)
{
	if(sdlImageInitializedCount == 0)
	{
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	}
	sdlImageInitializedCount++;

	surface = SDL_CreateRGBSurface(0, width, height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	if(surface == 0)
	{
		throw std::runtime_error("Could not create an image with dimensions of " + std::to_string(width) + "x" + std::to_string(height));
	}
}

Image::Image(std::string const & filename)
{
	if(sdlImageInitializedCount == 0)
	{
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	}
	sdlImageInitializedCount++;

	SDL_Surface * originalSurface = IMG_Load(filename.c_str());
	if(originalSurface == 0)
	{
		throw std::runtime_error("Could not load image \"" + filename + "\"");
	}
	SDL_PixelFormat pixelFormat = createPixelFormat(originalSurface->format->Amask != 0);
	surface = SDL_ConvertSurface(originalSurface, &pixelFormat, 0);
	SDL_FreeSurface(originalSurface);
}

Image::~Image()
{
	SDL_FreeSurface((SDL_Surface *)surface);

	sdlImageInitializedCount--;
	if(sdlImageInitializedCount == 0)
	{
		IMG_Quit();
	}
}

void Image::save(std::string const & filename)
{
	if(filename.find_last_of(".png") != filename.size() - 1)
	{
		throw std::runtime_error("Only saving of PNG files is currently supported.");
	}
	IMG_SavePNG((SDL_Surface *)surface, filename.c_str());
}

