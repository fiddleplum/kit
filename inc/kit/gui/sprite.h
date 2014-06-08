#pragma once

#include <kit/gui/widget.h>

namespace kit
{
	namespace gui
	{
		class Sprite : virtual public Widget
		{
		public:
			virtual void setTexture(std::string const & filename) = 0;

			virtual void setTextureBounds(Recti bounds) = 0;

			virtual Recti getTextureBounds() const = 0;
		};
	}
}

