#pragma once

#include "resource_manager.h"
#include <kit/resources.h>

namespace kit
{
	class TextureP;
	class Shader;
	namespace scene
	{
		class ModelP;
	}

	class ResourcesP : public Resources
	{
	public:
		ResourcesP ();

		Ptr<Texture> getNewTexture (std::string const & name, Vector2i size) override;

		Ptr<Texture> getTextureFromFile (std::string const & filename) override;

		Ptr<scene::Model> getNewModel (std::string const & name) override;

		Ptr<scene::Model> getModelFromFile (std::string const & filename) override;

		Ptr<Shader> getShader (std::string const & name, std::string code []);

	private:
		OwnPtr<ResourceManager<TextureP>> textureManager;
		OwnPtr<ResourceManager<scene::ModelP>> modelManager;
		OwnPtr<ResourceManager<Shader>> shaderManager;
	};
}

