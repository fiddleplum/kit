#pragma once

#include "gui_element_impl.h"
#include <kit/gui_sprite.h>
#include <kit/ptr.h>
#include <string>

// TODO: Add ability to have a texture mask for determining the shape of the sprite when it comes to cursor clicks, etc.
// TODO: Add ability to have a base color (yellow, blue, transparent, etc) as vertex color that the texture lays on top of.
//   This would require modifying the gui shader.

class Texture;
class GuiModel;

class GuiSpriteImpl : virtual public GuiElementImpl, virtual public GuiSprite
{
public:
	GuiSpriteImpl();

	Recti getBounds() const override;

	void setPosition(Vector2i position) override;

	Recti getTextureBounds() const override;

	void setTextureBounds(Recti bounds) override;

	void setTexture(std::string const & name, Image const & image) override;

	void setTexture(std::string const & filename) override;

	Image getImage() const override;

	void render(Vector2i windowSize) const override;

private:
	void updateVertices();

	OwnPtr<GuiModel> model;
	Vector2i position;
	Recti textureBounds;
};

