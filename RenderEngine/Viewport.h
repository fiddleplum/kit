#pragma once

#include "../Box.h"

// This is a widget. When we get widgets

class Viewport
{
public:
	Viewport();

	void setWindowSize(Vector2i);

	Box2i getBounds() const;

	void setBounds(Box2i);

	Camera * getCamera() const;

	void setCamera(Camera *);

	void render() const;

private:
	Vector2i windowSize;
	Box2i bounds;
	Camera * camera;
};

