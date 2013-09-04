#pragma once

#include "Box.h"
#include "Widget.h"

class Camera;

class Viewport : public Widget
{
public:
	Viewport();

	Box2i getBounds() const;
	void setBounds(Box2i);

	Camera * getCamera() const;
	void setCamera(Camera *);

	void render() const override;

private:
	Box2i bounds;
	Camera * camera;
};

