#pragma once

#include "texture.h"
#include "gui_element.h"
#include "ptr.h"
#include <functional>

class GuiSprite;

class GuiButton : public GuiElement
{
public:
	enum Type
	{
		Hold, Toggle, Shot
	};

	GuiButton();

	Recti getBounds() const override;

	void setPosition(Vector2i position) override;

	bool handleEvent(Event const & event, Vector2i cursorPosition, bool cursorPositionIsValid) override;

	void render(Vector2i windowSize) const override;

	Ptr<Texture> getTexture() const;

	// Sets the texture to use.
	void setTexture(Ptr<Texture> texture);

	// Sets the texture to use from a filename.
	void setTexture(std::string const & filename);

	// Frames are stacked horizontally: default, hovered, pressed, pressed-hovered.
	void setTextureBoundsOfDefaultState(Recti bounds);

	// Set the type of button.
	void setType(Type type);

	// Set how long the button will stay pressed, if the shot type.
	void setShotInterval(float interval);

	// Sets if the button is selected.
	void setSelected(bool selected);

	// Sets a function to be called when the button becomes pressed.
	void setPressFunction(std::function<void()> pressFunction);

	// Sets a function to be called when the button comes back up (from either mouse up or a second press on a toggle button).
	void setUnpressFunction(std::function<void()> unpressFunction);

private:
	void setSpriteTextureBoundsFromState();

	std::function<void()> pressFunction;
	std::function<void()> unpressFunction;
	Type type;
	float shotInterval;
	bool hovered;
	bool pressed;
	//bool toggled;
	float timePressed;
	OwnPtr<GuiSprite> sprite;
	Recti defaultTextureBounds;
};

