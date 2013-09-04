#include "Viewport.h"
#include "OpenGL.h"
#include "Camera.h"
#include "App.h"

Viewport::Viewport()
{
	bounds = Box2i::zero();
}

Box2i Viewport::getBounds() const
{
	return bounds;
}

void Viewport::setBounds(Box2i newBounds)
{
	bounds = newBounds;
	if(camera != nullptr)
	{
		Vector2i size = bounds.getSize();
		if(size[1] != 0)
		{
			camera->setAspectRatio((float)size[0] / (float)size[1]);
		}
	}
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
	Vector2i windowSize = App::getSize();
	glViewport(bounds.min[0], windowSize[1] - bounds.max[1], bounds.max[0] - bounds.min[0], bounds.max[1] - bounds.min[1]);

	if(camera != nullptr)
	{
		camera->render();
	}

	glViewport(0, 0, windowSize[0], windowSize[1]);
}

