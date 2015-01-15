#include "gui_viewport.h"
#include "open_gl.h"

GuiViewport::GuiViewport()
{
}

Recti GuiViewport::getBounds() const
{
	return bounds;
}

void GuiViewport::setPosition(Coord2i position)
{
	bounds.setMinKeepSize(position);
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

void GuiViewport::render(Coord2i windowSize) const
{
	if(!camera.isValid() || !scene.isValid())
	{
		return;
	}

	glViewport(bounds.min[0], windowSize[1] - bounds.max[1], bounds.max[0] - bounds.min[0], bounds.max[1] - bounds.min[1]);
	scene->render(camera);
	glViewport(0, 0, windowSize[0], windowSize[1]);
}

