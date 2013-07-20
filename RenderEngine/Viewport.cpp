#include "Viewport.h"
#include "OpenGL.h"
#include "Camera.h"

Viewport::Viewport()
{
	windowSize = Vector2i::zero();
	bounds = Box2i::zero();
}

Box2i Viewport::getBounds() const
{
	return bounds;
}

void Viewport::setBounds(Box2i newBounds)
{
	bounds = newBounds;
}

Camera * Viewport::getCamera() const
{
	return camera;
}

void Viewport::setCamera(Camera * newCamera)
{
	camera = newCamera;
}

void Viewport::render() const
{
	glViewport(bounds.min[0], windowSize[1] - bounds.max[1], bounds.max[0] - bounds.min[0], bounds.max[1] - bounds.min[1]);

	camera->render();

	glViewport(0, 0, windowSize[0], windowSize[1]);
}

