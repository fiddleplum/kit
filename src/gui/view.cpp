#include "view.h"
#include "../app.h"
#include "../open_gl.h"
#include "../camera.h"
#include "../scene.h"

namespace app
{
	View::View()
	{
		bounds = Recti::zero();
	}

	Recti View::getBounds() const
	{
		return bounds;
	}

	void View::setPosition(Vector2i position)
	{
		bounds.setMinKeepSize(position);
	}

	void View::setMaxSize(Vector2i maxSize)
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

	void View::handleEvent(Event const & event)
	{
		if(scene != nullptr)
		{
			scene->handleEvent(event);
		}
	}

	std::shared_ptr<Camera> View::getCamera() const
	{
		return camera;
	}

	void View::setCamera(std::shared_ptr<Camera> newCamera)
	{
		camera = newCamera;
		if(bounds.getSize()[1] != 0)
		{
			camera->setAspectRatio((float)bounds.getSize()[0] / (float)bounds.getSize()[1]);
		}
	}

	std::shared_ptr<Scene> View::getScene() const
	{
		return scene;
	}

	void View::setScene(std::shared_ptr<Scene> newScene)
	{
		scene = newScene;
	}

	void View::render()
	{
		if(camera == nullptr || scene == nullptr)
		{
			return;
		}

		Vector2i windowSize = App::getSize();
		glViewport(bounds.min[0], windowSize[1] - bounds.max[1], bounds.max[0] - bounds.min[0], bounds.max[1] - bounds.min[1]);

		scene->render(camera);

		glViewport(0, 0, windowSize[0], windowSize[1]);
	}
}

