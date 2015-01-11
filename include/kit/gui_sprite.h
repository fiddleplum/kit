#pragma once

#include "gui_element.h"
#include "image.h"
#include <string>

class GuiSprite : virtual public GuiElement
{
public:
	virtual Recti getBounds() const = 0;

	virtual void setPosition(Vector2i position) = 0;

	virtual Recti getTextureBounds() const = 0;

	virtual void setTextureBounds(Recti bounds) = 0;

	virtual void setTexture(std::string const & name, Image const & image) = 0;

	virtual void setTexture(std::string const & filename) = 0;

	virtual Image getImage() const = 0;
};

