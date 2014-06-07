#pragma once

#include "sprite_internal.h"
#include "widget_internal.h"
#include <kit/gui/button.h>

namespace kit
{
	namespace gui
	{
		class ButtonInternal : virtual public Button, virtual public WidgetInternal
		{
		public:
			ButtonInternal ();

			Recti getBounds () const override;

			void setPosition (Vector2i position) override;

			void setMaxSize (Vector2i maxSize) override;

			bool handleEvent (Event const & event, bool cursorIsValid) override;

			void render (Vector2i windowSize) override;

			void setTexture (std::string const & filename) override;
			void setTextureBounds (Recti bounds) override; // Frames are stacked horizontally: default, hovered, pressed, pressed-hovered

			void setType (Type type) override;
			void setShotInterval (float interval) override;
			void setHoverFunction (std::function<void ()> hoverFunction) override;
			void setUnhoverFunction (std::function<void ()> unhoverFunction) override;
			void setPressFunction (std::function<void ()> pressFunction) override;
			void setUnpressFunction (std::function<void ()> unpressFunction) override;

			bool isHovered () const;
			bool isPressed () const;

		private:
			void setSpriteTextureBoundsFromState ();

			std::function<void ()> hoverFunction;
			std::function<void ()> unhoverFunction;
			std::function<void ()> pressFunction;
			std::function<void ()> unpressFunction;
			Type type;
			float shotInterval;
			bool hovered;
			bool pressed;
			bool toggled;
			float timePressed;
			OwnPtr<SpriteInternal> sprite;
			Recti defaultTextureBounds;
		};
	}
}

