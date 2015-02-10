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
	bounds.max += position - bounds.min;
	bounds.min = position;
}

void GuiViewport::setSize(Coord2i size)
{
	bounds.max = bounds.min + size - Coord2i{1, 1};
}

Ptr<SceneCamera> GuiViewport::getCamera() const
{
	return camera;
}

void GuiViewport::setCamera(Ptr<SceneCamera> camera)
{
	this->camera = camera;
	if(bounds.max[1] - bounds.min[1] + 1 != 0)
	{
		camera->setAspectRatio((float)(bounds.max[0] - bounds.min[0] + 1) / (float)(bounds.max[1] - bounds.min[1] + 1));
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

