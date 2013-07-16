#pragma once

#include <string>

class Texture
{
public:
	// Constructs from a file.
	Texture(std::string const & filename);

	// Destructs.
	~Texture();

	// Activates the texture at the slot.
	void activate(unsigned int slot);

	// Deactivates all slots equal to or greater than slot.
	static void deactivateRest(unsigned int slot);

private:
	unsigned int mHandle;
};

