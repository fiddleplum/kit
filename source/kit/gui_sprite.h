#pragma once

#include "texture.h"
#include "event.h"
#include "gui_element.h"
#include "ptr.h"
#include <string>

// TODO: Add ability to have a texture mask for determining the shape of the sprite when it comes to cursor clicks, etc.
// TODO: Add ability to have a base color (yellow, blue, transparent, etc) as vertex color that the texture lays on top of.
//   This would require modifying the gui shader.

class GuiModel;

class GuiSprite : public GuiElement
{
public:
	GuiSprite();

	Recti getBounds() const;

	void setPosition(Vector2i position);

	Recti getTextureBounds() const;

	void setTextureBounds(Recti bounds);

	Ptr<Texture> getTexture() const;

	void setTexture(Ptr<Texture> texture);

	void setTexture(std::string const & filename);

	void render(Vector2i windowSize) const override;

private:
	void updateVertices();

	OwnPtr<GuiModel> model;
	Vector2i position;
	Recti textureBounds;
	Recti bounds;
};

