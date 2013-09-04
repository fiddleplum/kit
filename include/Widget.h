#pragma once

#include "Vector.h"
#include "Box.h"

class Widget
{
public:
	/// Gets the bounds of the widget.
	virtual Box2i getBounds() const = 0;

	/// Sets the position of the widget.
	virtual void setPosition(Vector2i) = 0;

	/// Sets the maximum size this widget can be. The widget can max out to this size (text box), or not change its size at all (check box).
	virtual void setMaxSize(Vector2i) = 0;

	/// Renders the widget to the screen.
	virtual void render() const = 0;
};

