#pragma once

#include <kit/ptr.h>
#include <kit/gui/widget.h>
#include <string>

namespace kit
{
	class Texture;

	namespace gui
	{
		class Sprite : virtual public Widget
		{
		public:
			virtual Ptr<Texture> getTexture () const = 0;

			virtual void setNewTexture (Vector2i size) = 0;

			virtual void setTexture (std::string const & filename) = 0;

			virtual void setTextureBounds (Recti bounds) = 0;

			virtual Recti getTextureBounds () const = 0;
		};
	}
}

