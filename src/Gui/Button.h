#pragma once

#include "Sprite.h"
#include "Widget.h"
#include <functional>

namespace Gui
{
	class Button : public Widget
	{
	public:
		enum Type
		{
			Hold, Toggle, Shot
		};

		Button();

		Box2i getBounds() const override;
		void setPosition(Vector2i position) override;
		void setMaxSize(Vector2i maxSize) override;
		void handleEvent(Event const & event) override;
		void render() override;

		void setTexture(std::string const & filename);
		void setTextureBounds(Box2i bounds); // Frames are stacked horizontally: default, hovered, pressed, pressed-hovered

		void setType(Type type);
		void setShotInterval(float interval);
		void setHoverFunction(std::function<void ()> hoverFunction);
		void setUnhoverFunction(std::function<void ()> unhoverFunction);
		void setPressFunction(std::function<void ()> pressFunction);
		void setUnpressFunction(std::function<void ()> unpressFunction);

		bool isHovered() const;
		bool isPressed() const;

	private:
		void setSpriteTextureBoundsFromState();

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
		Sprite sprite;
		Box2i defaultTextureBounds;
	};
}
