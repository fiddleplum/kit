#include "gui_viewport.h"
#include "open_gl.h"
#include "cursor.h"
#include "window.h"

GuiViewport::GuiViewport()
{
}

Recti GuiViewport::getBounds() const
{
	return bounds;
}

void GuiViewport::setPosition(Vector2i position)
{
	bounds.setMinKeepSize(position);
}

void GuiViewport::setClipBounds(Recti clipBounds)
{
	this->clipBounds = clipBounds;
}

void GuiViewport::setMaxCoord(Vector2i maxCoord)
{
	_bounds.max = maxCoord;
	if(_camera.isValid())
	{
		if(_bounds.getSize()[1] != 0)
		{
			_camera->setAspectRatio((float)_bounds.getSize()[0] / (float)_bounds.getSize()[1]);
		}
	}
}

Ptr<SceneCamera> GuiViewport::getCamera() const
{
	return camera;
}

void GuiViewport::setCamera(Ptr<SceneCamera> camera)
{
	this->camera = camera;
	if(bounds.getSize()[1] != 0)
	{
		camera->setAspectRatio((float)bounds.getSize()[0] / (float)bounds.getSize()[1]);
	}
}

Ptr<Scene> GuiViewport::getScene() const
{
	return scene;
}

void GuiViewport::setScene(Ptr<Scene> scene)
{
	this->scene = scene;
}

void GuiViewport::handleEvent(Event const & event)
{
	if(event.window.isValid())
	{
		Ptr<Cursor> cursor = event.window->getCursor();
		if(cursor->isPositionValid() && getBounds().containsEx(cursor->getPosition()))
		{
			cursor->consume();
		}
	}
}

void GuiViewport::render(Vector2i windowSize)
{
	if(!camera.isValid() || !scene.isValid())
	{
		return;
	}


	glViewport(bounds.min[0], windowSize[1] - bounds.max[1], bounds.max[0] - bounds.min[0], bounds.max[1] - bounds.min[1]);
	scene->render(camera);
	glViewport(0, 0, windowSize[0], windowSize[1]);
}

