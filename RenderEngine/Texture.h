#pragma once

#include <string>

class Texture
{
public:
	Texture(std::string const & filename);

	~Texture();

	std::string toString() const;

	void activate(unsigned int slot);
	
	static void deactivate(unsigned int slot);

private:
	std::string mFilename;
	unsigned int mHandle;
};

