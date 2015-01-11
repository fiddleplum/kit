#pragma once

#include "rect.h"
#include "vector.h"

class GuiElement
{
public:
	// Virtual destructor, so it can be deleted properly.
	virtual ~GuiElement() {}

	// Returns the bounds of the element.
	virtual Recti getBounds() const = 0;

	// Sets the position of the element.
	virtual void setPosition(Vector2i position) = 0;
};

