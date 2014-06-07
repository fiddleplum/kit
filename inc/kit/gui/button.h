#pragma once

#include <kit/gui/widget.h>
#include <functional>

namespace kit
{
	namespace gui
	{
		class Button : virtual public Widget
		{
		public:
			enum Type
			{
				Hold, Toggle, Shot
			};

			virtual void setTexture (std::string const & filename) = 0;

			// Frames are stacked horizontally: default, hovered, pressed, pressed-hovered.
			virtual void setTextureBounds (Recti bounds) = 0;

			virtual void setType (Type type) = 0;

			virtual void setShotInterval (float interval) = 0;

			virtual void setHoverFunction (std::function<void ()> hoverFunction) = 0;

			virtual void setUnhoverFunction (std::function<void ()> unhoverFunction) = 0;

			virtual void setPressFunction (std::function<void ()> pressFunction) = 0;

			virtual void setUnpressFunction (std::function<void ()> unpressFunction) = 0;
		};
	}
}

