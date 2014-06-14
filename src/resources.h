#pragma once

#include "resource_manager.h"

namespace kit
{
	class TextureP;
	class Shader;
	class ModelP;

	class Resources
	{
	public:
		Resources ();

		Ptr<ResourceManager<TextureP>> textures ();

		Ptr<ResourceManager<Shader>> shaders ();

		Ptr<ResourceManager<ModelP>> models ();

	private:
		OwnPtr<ResourceManager<TextureP>> textureManager;
		OwnPtr<ResourceManager<ModelP>> modelManager;
		OwnPtr<ResourceManager<Shader>> shaderManager;
	};
}

