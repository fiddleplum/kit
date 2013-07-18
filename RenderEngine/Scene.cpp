#include "Scene.h"
#include "Model.h"
#include "Camera.h"

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

void Scene::render()
{
	// Set the OpenGL settings.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Check for sorting.
	std::vector<std::shared_ptr<Model>> modelsToInsert;
	for(std::shared_ptr<Model> model : mModels)
	{
		if(model->needsResorting())
		{
			mModels.erase(model);
			modelsToInsert.push_back(model);
		}
	}
	for(std::shared_ptr<Model> model : modelsToInsert)
	{
		mModels.insert(model);
		model->resortingDone();
	}

	std::shared_ptr<Shader const> currentShader = nullptr;
	std::vector<std::shared_ptr<Texture const>> currentTextures;
	for(std::shared_ptr<Model> model : mModels)
	{
		// Activate a new shader.
		std::shared_ptr<Shader> modelShader = model->getShader();
		if(modelShader != currentShader)
		{
			modelShader->activate();
			currentShader = modelShader;
		}

		// Activate new textures.
		std::vector<std::shared_ptr<Texture>> const & modelTextures = model->getTextures();
		unsigned int slot = 0;
		for(slot = 0; slot < modelTextures.size(); slot++)
		{
			if(slot >= currentTextures.size() || modelTextures[slot] != currentTextures[slot])
			{
				modelTextures[slot]->activate(slot);
				if(slot >= currentTextures.size())
				{
					currentTextures.resize(slot + 1);
				}
				currentTextures[slot] = modelTextures[slot];
			}
		}

		// Deactivate unused textures.
		for(; slot < currentTextures.size(); slot++)
		{
			currentTextures[slot]->deactivate(slot);
			currentTextures[slot] = nullptr;
		}

		// Do the render.
		model->render();
	}
	if(currentShader != nullptr)
	{
		currentShader->deactivate();
	}
}

