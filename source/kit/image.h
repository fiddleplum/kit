#pragma once

#pragma comment (lib, "SDL2_image.lib")

#include <string>

class Image
{
public:
	Image(int width, int height);
	Image(std::string const & filename);
	~Image();
	void save(std::string const & filename);

private:
	void * surface;
};

