#pragma once

#include <kit/texture.h>
#include <string>

namespace kit
{
	class TextureP : virtual public Texture
	{
	public:
		TextureP (Vector2i size);

		TextureP (std::string const & filename);

		~TextureP ();

		virtual Vector2i getSize () const override;

		virtual unsigned char * lockPixels () override;

		virtual void unlockPixels () override;

		// Activates the texture in the GL slot.
		void activate (unsigned int slot) const;

		// Deactivates all slots equal to or greater than the GL slot.
		static void deactivateRest (unsigned int slot);

	private:
		Vector2i size;
		unsigned int id;
	};
}

