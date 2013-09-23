#include "Scene.h"
#include "Model.h"
#include "Camera.h"
#include "OpenGL.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

Model * Scene::createModel()
{
	Model * model = new Model(this);
	mModels.insert(model);
	return model;
}

void Scene::destroyModel(Model * model)
{
	mModels.erase(model);
	delete model;
}

Camera * Scene::createCamera()
{
	Camera * camera = new Camera(this);
	mCameras.insert(camera);
	return camera;
}

void Scene::destroyCamera(Camera * camera)
{
	mCameras.erase(camera);
	delete camera;
}

void Scene::handleEvent(Event const & event)
{
}

void Scene::render(Camera * camera)
{
	// Set the OpenGL settings.
	glEnable(GL_DEPTH_TEST);

	// Check for sorting. Pull out the ones that need to be resorted, and put them back in the proper place.
	std::vector<Model *> modelsToInsert;
	for(auto it = mModels.begin(); it != mModels.end();)
	{
		Model * model = *it;
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
	for(Model * model : modelsToInsert)
	{
		mModels.insert(model);
		model->resortingDone();
	}

	// Do the render.
	for(Model * model : mModels)
	{
		model->render(camera);
	}
}

bool Scene::ModelCompare::operator ()(Model const * model0, Model const * model1)
{
	return *model0 < *model1;
}
