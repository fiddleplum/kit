#pragma once

#include "gui_element.h"
#include "event.h"
#include "ptr.h"
#include <functional>

class GuiSprite;

class GuiContainer : virtual public GuiElement
{
public:
	// Sets the size of the container. Within the bounds all elements are clipped.
	virtual void setSize(Vector2i size) = 0;

	// Sets the handler for when container-wide events occur. See GuiElement for handler functionality.
	virtual void setEventHandler(std::function<bool (Event const & event, Vector2i cursorPosition, bool cursorPositionIsValid)> handler) = 0;

	// Adds a sprite element.
	virtual Ptr<GuiSprite> addSprite() = 0;

	// Removes an element.
	virtual void removeElement(Ptr<GuiElement> element);

	// Moves the element to the front.
	virtual void moveElementToFront(Ptr<GuiElement> element) = 0;

	// Does the element handle events and rendered?
	virtual bool isElementActive(Ptr<GuiElement> element) const = 0;

	// Sets whether the element handles events and rendered.
	virtual void setElementActive(Ptr<GuiElement> element, bool active) = 0;
};

