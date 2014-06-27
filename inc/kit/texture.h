#pragma once

#include <kit/vector.h>
#include <string>

namespace kit
{
	class Texture
	{
	public:
		// Creates a blank (black and 0 alpha) texture of the given size.
		Texture (Vector2i size);

		// Loads a texture from the file.
		Texture (std::string const & filename);

		// Destructor.
		~Texture ();

		// Returns the non-power-of-two size of the texture.
		Vector2i getSize () const;

		// Locks the pixels for editing.
		unsigned char * lockPixels ();

		// Unlocks the pixels when done with editing.
		void unlockPixels ();

		//////// Internal

		// Activates the texture in the GL slot.
		void activate (unsigned int slot) const;

		// Deactivates all slots equal to or greater than the GL slot.
		static void deactivateRest (unsigned int slot);

	private:
		Vector2i _size;
		unsigned int _id;
	};
}

