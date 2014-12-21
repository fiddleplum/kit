#pragma once

#include "texture.h"
#include "event.h"
#include "gui_element.h"
#include "ptr.h"
#include <string>

// TODO: Add ability to have a texture mask for determining the shape of the sprite when it comes to cursor clicks, etc.

class GuiModel;

class GuiSprite : public GuiElement
{
public:
	GuiSprite();

	bool canHaveFocus() override;

	bool hasFocus() const override;

	void setFocus(bool value) override;

	// Returns the unclipped bounds.
	Recti getBounds() const;

	void setPosition(Vector2i position);

	Recti getTextureBounds() const;

	void setTextureBounds(Recti bounds);

	void setClipBounds(Recti clipBounds) override;

	Ptr<Texture> getTexture() const;

	void setTexture(Ptr<Texture> texture);

	void setTexture(std::string const & filename);

	bool handleEvent(Event const & event, Ptr<Cursor> cursor) override;

	void render(Vector2i windowSize) const override;

private:
	void updateVertices();

	OwnPtr<GuiModel> model;
	Vector2i position;
	Recti clipBounds;
	Recti textureBounds;
	Recti visibleBounds; // intersection between real bounds and clipped bounds
};

