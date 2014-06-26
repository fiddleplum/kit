#pragma once

#include <kit/ptr.h>
#include <kit/gui/widget.h>
#include <kit/texture.h>
#include <string>

namespace kit
{
	namespace gui
	{
		class Sprite : virtual public Widget
		{
		public:
			static OwnPtr<Sprite> create ();

			virtual UsePtr<Texture> getTexture () const = 0;

			virtual void setTexture (UsePtr<Texture> texture) = 0;

			virtual void setTexture (std::string const & filename) = 0;

			virtual void setTextureBounds (Recti bounds) = 0;

			virtual Recti getTextureBounds () const = 0;
		};
	}
}

