#pragma once

#include "../texture.h"
#include "../ptr.h"
#include "widget.h"
#include <functional>

namespace kit
{
	namespace gui
	{
		class Sprite;

		class Button : public Widget
		{
		public:
			enum Type
			{
				Hold, Toggle, Shot
			};

			Button();

			Recti getBounds() const override;

			void setPosition(Vector2i position) override;

			void setMaxCoord(Vector2i maxCoord) override;

			void handleEvent(Event const & event) override;

			void render(Vector2i windowSize) override;

			Ptr<Texture> getTexture() const;

			// Sets the texture to use.
			void setTexture(Ptr<Texture> texture);

			// Sets the texture to use from a filename.
			void setTexture(std::string const & filename);

			// Frames are stacked horizontally: default, hovered, pressed, pressed-hovered.
			void setTextureBounds(Recti bounds);

			// Set the type of button.
			void setType(Type type);

			// Set how long the button will stay pressed, if the shot type.
			void setShotInterval(float interval);

			// Sets if the button is selected.
			void setSelected(bool selected);

			// Set a function to be called when the cursor moves over the button.
			void setHoverFunction(std::function<void()> hoverFunction);

			// Set a function to be called when the cursor moves away from the button.
			void setUnhoverFunction(std::function<void()> unhoverFunction);

			// Sets a function to be called when the button becomes pressed.
			void setPressFunction(std::function<void()> pressFunction);

			// Sets a function to be called when the button comes back up (from either mouse up or a second press on a toggle button).
			void setUnpressFunction(std::function<void()> unpressFunction);

		private:
			void press();
			void setSpriteTextureBoundsFromState();

			std::function<void()> hoverFunction;
			std::function<void()> unhoverFunction;
			std::function<void()> pressFunction;
			std::function<void()> unpressFunction;
			Type type;
			float shotInterval;
			bool selected;
			bool hovered;
			bool pressed;
			bool toggled;
			float timePressed;
			OwnPtr<Sprite> sprite;
			Recti defaultTextureBounds;
		};
	}
}

