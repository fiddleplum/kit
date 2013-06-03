#pragma once

#include <string>
#include <memory>

class Texture
{
public:
	Texture(std::string const & filename);

	class Resource;

private:
	std::shared_ptr<Resource> mResource;
};

