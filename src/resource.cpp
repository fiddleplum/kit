#include "resource.h"
#include "texture.h"
#include "shader.h"

namespace kit
{
	OwnPtr<ResourceManager<Texture>> textureManager;
	OwnPtr<ResourceManager<Shader>> shaderManager;

	void startResourceManagers ()
	{
		textureManager.set(new ResourceManager<Texture>);
		shaderManager.set(new ResourceManager<Shader>);
	}

	void finishResourceManagers ()
	{
		textureManager.setNull();
		shaderManager.setNull();
	}

	Ptr<ResourceManager<Texture>> getTextureManager()
	{
		return textureManager;
	}

	Ptr<ResourceManager<Shader>> getShaderManager()
	{
		return shaderManager;
	}
}

