#include "resources_p.h"
#include "texture_p.h"
#include "shader.h"
#include "scene/model_p.h"

namespace kit
{
	ResourcesP::ResourcesP ()
	{
		textureManager.set(new ResourceManager<TextureP>);
		shaderManager.set(new ResourceManager<Shader>);
		modelManager.set(new ResourceManager<scene::ModelP>);
	}

	Ptr<Texture> ResourcesP::getNewTexture (std::string const & name, Vector2i size)
	{
		return textureManager->get(name, size);
	}

	Ptr<Texture> ResourcesP::getTextureFromFile (std::string const & filename)
	{
		return textureManager->get(filename, filename);
	}

	Ptr<scene::Model> ResourcesP::getNewModel (std::string const & name)
	{
		return modelManager->get(name);
	}

	Ptr<scene::Model> ResourcesP::getModelFromFile (std::string const & filename)
	{
		return modelManager->get(filename, filename);
	}

	Ptr<Shader> ResourcesP::getShader (std::string const & name, std::string code [])
	{
		return shaderManager->get(name, code);
	}
}

