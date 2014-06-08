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

			// Sets the texture to use.
			virtual void setTexture (std::string const & filename) = 0;

			// Frames are stacked horizontally: default, hovered, pressed, pressed-hovered.
			virtual void setTextureBounds (Recti bounds) = 0;

			// Set the type of button.
			virtual void setType (Type type) = 0;

			// Set how long the button will stay pressed, if the shot type.
			virtual void setShotInterval (float interval) = 0;

			// Set a function to be called when the cursor moves over the button.
			virtual void setHoverFunction (std::function<void ()> hoverFunction) = 0;

			// Set a function to be called when the cursor moves away from the button.
			virtual void setUnhoverFunction (std::function<void ()> unhoverFunction) = 0;

			// Sets a function to be called when the button becomes pressed.
			virtual void setPressFunction (std::function<void ()> pressFunction) = 0;

			// Sets a function to be called when the button comes back up (from either mouse up or a second press on a toggle button).
			virtual void setUnpressFunction (std::function<void ()> unpressFunction) = 0;
		};
	}
}

