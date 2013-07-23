#pragma once

#include "../Vector.h"

class Widget;

class Gui
{
public:
	Gui();
	void setWindowSize(Vector2i);
	void setWidget(Widget *);
	void render() const;

private:
	Vector2i windowSize;
	Widget * widget;
};

