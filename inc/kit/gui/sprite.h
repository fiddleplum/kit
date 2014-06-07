#pragma once

#include <kit/gui/widget.h>

namespace kit
{
	namespace gui
	{
		class Sprite : virtual public Widget
		{
		public:
			virtual Recti getBounds() const = 0;

			virtual void setPosition(Vector2i position) = 0;

			virtual void setMaxSize(Vector2i maxSize) = 0;

			virtual void setTexture(std::string const & filename) = 0;

			virtual void setTextureBounds(Recti bounds) = 0;

			virtual Recti getTextureBounds() const = 0;
		};
	}
}

