#pragma once

#include "Widget.h"

class Camera;

class Viewport : public Widget
{
public:
	Viewport();

	// Inherited from Widget.
	Box2i getBounds() const override;
	void setPosition(Vector2i position) override;
	void setMaxSize(Vector2i maxSize) override;
	void handleEvent(Event const & event) override;
	void render() override;

	// You can attach a camera.
	Camera * getCamera() const;
	void setCamera(Camera *);

private:
	Box2i bounds;
	Camera * camera;
};

