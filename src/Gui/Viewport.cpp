#include "Viewport.h"
#include "../OpenGL.h"
#include "../Camera.h"
#include "../App.h"

Viewport::Viewport()
{
	bounds = Box2i::zero();
}

Box2i Viewport::getBounds() const
{
	return bounds;
}

void Viewport::setPosition(Vector2i position)
{
	bounds.setMinKeepSize(position);
}

void Viewport::setMaxSize(Vector2i maxSize)
{
	bounds.setSize(maxSize);
	if(camera != nullptr)
	{
		if(maxSize[1] != 0)
		{
			camera->setAspectRatio((float)maxSize[0] / (float)maxSize[1]);
		}
	}
}

void Viewport::handleEvent(Event const & event)
{
	if(camera != nullptr)
	{
		camera->handleEvent(event);
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

void Viewport::render()
{
	Vector2i windowSize = App::getSize();
	glViewport(bounds.min[0], windowSize[1] - bounds.max[1], bounds.max[0] - bounds.min[0], bounds.max[1] - bounds.min[1]);

	if(camera != nullptr)
	{
		camera->render();
	}

	glViewport(0, 0, windowSize[0], windowSize[1]);
}

