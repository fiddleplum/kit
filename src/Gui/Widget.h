#pragma once

#include "../Vector.h"
#include "../Box.h"
#include "../Event.h"

class Widget
{
public:
	// Empty default constructor.
	Widget() {}

	// Virtual destructor for subclasses.
	virtual ~Widget() {}

	// Gets the bounds of the widget.
	virtual Box2i getBounds() const = 0;

	// Sets the position of the widget.
	virtual void setPosition(Vector2i position) = 0;

	// Sets the maximum size this widget can be. The widget can max out to this size (text box), or not change its size at all (check box).
	virtual void setMaxSize(Vector2i maxSize) = 0;

	// Handles events.
	virtual void handleEvent(Event const & event) = 0;

	// Renders the widget.
	virtual void render() = 0;

private:
	// TODO: Make these public and with the delete specifier, once MSVC upgrades.
	/// Not copy-constructable.
	Widget(Widget const &);

	/// Not copyable.
	Widget & operator = (Widget const &);
};

