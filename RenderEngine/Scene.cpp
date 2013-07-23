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

ResourceManager<Texture> & Scene::getTextureManager()
{
	return mTextureManager;
}

ResourceManager<Shader> & Scene::getShaderManager()
{
	return mShaderManager;
}

void Scene::render(Camera * camera)
{
	// Set the OpenGL settings.
	glEnable(GL_DEPTH_TEST);

	// Check for sorting.
	std::vector<Model *> modelsToInsert;
	for(Model * model : mModels)
	{
		if(model->needsResorting())
		{
			mModels.erase(model);
			modelsToInsert.push_back(model);
		}
	}
	for(Model * model : modelsToInsert)
	{
		mModels.insert(model);
		model->resortingDone();
	}

	for(Model * model : mModels)
	{
		model->render(camera);
	}
}

bool Scene::ModelCompare::operator ()(Model const * model0, Model const * model1)
{
	return *model0 < *model1;
}
