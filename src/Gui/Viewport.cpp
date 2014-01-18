#include "Viewport.h"
#include "../App.h"
#include "../OpenGL.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"

namespace Gui
{
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
		if(scene != nullptr)
		{
			scene->handleEvent(event);
		}
	}

	std::shared_ptr<Scene::Camera> Viewport::getCamera() const
	{
		return camera;
	}

	void Viewport::setCamera(std::shared_ptr<Scene::Camera> newCamera)
	{
		camera = newCamera;
		if(bounds.getSize()[1] != 0)
		{
			camera->setAspectRatio((float)bounds.getSize()[0] / (float)bounds.getSize()[1]);
		}
	}

	std::shared_ptr<Scene::Scene> Viewport::getScene() const
	{
		return scene;
	}

	void Viewport::setScene(std::shared_ptr<Scene::Scene> newScene)
	{
		scene = newScene;
	}

	void Viewport::render()
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

