#include "RenderEngine.h"

void RenderEngine::addModel(Model * model)
{
	mModels.insert(model);
}

void RenderEngine::removeModel(Model * model)
{
	mModel.erase(model);
}

ResourceManager<Texture, std::string const &> & RenderEngine::getTextureManager()
{
	return mTextureManager;
}

void RenderEngine::render() const
{
}

