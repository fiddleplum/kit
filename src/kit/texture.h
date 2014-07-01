#pragma once

#include "vector.h"
#include <string>

namespace kit
{
	class Texture
	{
	public:
		Texture (Vector2i size);

		Texture (std::string const & filename);

		~Texture ();

		Vector2i getSize () const;

		unsigned char * lockPixels ();

		void unlockPixels ();

		// Activates the texture in the GL slot. Used by Model.
		void activate (unsigned int slot) const;

		// Deactivates all slots equal to or greater than the GL slot. Used by Models.
		static void deactivateRest (unsigned int slot);

	private:
		Vector2i size;
		unsigned int id;
	};
}

