#include "RenderEngine.h"

void RenderEngine::addModel(Model * model)
{
	mModels.insert(model);
}

void RenderEngine::removeModel(Model * model)
{
	mModels.erase(model);
}

ResourceManager<Texture, std::string const &> & RenderEngine::getTextureManager()
{
	return mTextureManager;
}

void RenderEngine::render() const
{
	std::shared_ptr<Shader const> currentShader;
	std::set<std::shared_ptr<Texture const>> currentTextures;
}

