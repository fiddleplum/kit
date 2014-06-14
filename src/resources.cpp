#include "resources.h"
#include "resource_manager.h"
#include "texture_p.h"
#include "shader.h"
//#include "model_p.h"

namespace kit
{
	Resources::Resources ()
	{
		textureManager.set(new ResourceManager<TextureP>);
		shaderManager.set(new ResourceManager<Shader>);
		modelManager.set(new ResourceManager<ModelP>);
	}

	Ptr<ResourceManager<TextureP>> Resources::textures ()
	{
		return textureManager;
	}

	Ptr<ResourceManager<Shader>> Resources::shaders ()
	{
		return shaderManager;
	}

	Ptr<ResourceManager<ModelP>> Resources::models ()
	{
		return modelManager;
	}
}

