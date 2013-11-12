#include "Scene.h"
#include "Model.h"
#include "Camera.h"
#include "Controller.h"
#include "../OpenGL.h"

namespace Scene
{
	Scene::Scene()
	{
	}

	void Scene::addCamera(std::shared_ptr<Camera> camera)
	{
		cameras.insert(camera);
	}

	void Scene::removeCamera(std::shared_ptr<Camera> camera)
	{
		cameras.erase(camera);
	}

	void Scene::addModel(std::shared_ptr<Model> model)
	{
		mModels.insert(model);
	}

	void Scene::removeModel(std::shared_ptr<Model> model)
	{
		mModels.erase(model);
	}

	void Scene::addController(std::shared_ptr<Controller> controller)
	{
		controllers.insert(controller);
	}

	void Scene::removeController(std::shared_ptr<Controller> controller)
	{
		controllers.erase(controller);
	}

	void Scene::handleEvent(Event const & event)
	{
		for(auto controller : controllers)
		{
			controller->handleEvent(event);
		}
	}

	void Scene::render(std::shared_ptr<Camera> camera)
	{
		// Set the OpenGL settings.
		glEnable(GL_DEPTH_TEST);

		// Check for sorting. Pull out the ones that need to be resorted, and put them back in the proper place.
		std::vector<std::shared_ptr<Model>> modelsToInsert;
		for(auto it = mModels.begin(); it != mModels.end();)
		{
			std::shared_ptr<Model> model = *it;
			if(model->needsResorting())
			{
				it = mModels.erase(it);
				modelsToInsert.push_back(model);
			}
			else
			{
				it++;
			}
		}
		for(std::shared_ptr<Model> model : modelsToInsert)
		{
			mModels.insert(model);
			model->resortingDone();
		}

		// Do the render.
		for(std::shared_ptr<Model> model : mModels)
		{
			model->render(camera);
		}
	}

	bool Scene::ModelCompare::operator ()(std::shared_ptr<Model> model0, std::shared_ptr<Model> model1)
	{
		return *model0 < *model1;
	}
}

