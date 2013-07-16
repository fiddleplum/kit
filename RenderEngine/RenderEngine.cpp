#include "RenderEngine.h"
#include "Shader.h"
#include "Texture.h"

RenderEngine::RenderEngine()
{
}

RenderEngine::~RenderEngine()
{
}

std::shared_ptr<Model> RenderEngine::createModel()
{
	std::shared_ptr<Model> model;
	mModels.insert(model);
	return model;
}

void RenderEngine::destroyModel(std::shared_ptr<Model> model)
{
	mModels.erase(model);
}

ResourceManager<Texture> & RenderEngine::getTextureManager()
{
	return mTextureManager;
}

ResourceManager<Shader> & RenderEngine::getShaderManager()
{
	return mShaderManager;
}

void RenderEngine::render()
{
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

		model->render();
	}
	if(currentShader != nullptr)
	{
		currentShader->deactivate();
	}
}

