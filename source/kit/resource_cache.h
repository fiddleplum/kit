#pragma once

#include "object_cache.h"
#include "texture.h"
#include "shader.h"
#include "scene_model.h"

class ResourceCache
{
public:
	// If the named texture does not exist, it will create a blank texture.
	static Ptr<Texture> getTexture(std::string const & name, Vector2i size);

	// If the texture has not already been loaded, it will load the texture from a file.
	static Ptr<Texture> getTextureFromFile(std::string const & filename);

	// If the named shader has not already been created, it will create the texture.
	static Ptr<Shader> getShader(std::string const & name, std::string code[]);

	// If the named model does not exist, it will create a blank model.
	static Ptr<SceneModel> getModel(std::string const & name);

	// If the mode has not already been loaded, it will load the model from a file.
	static Ptr<SceneModel> getModelFromFile(std::string const & filename);

private:
	ResourceCache();
	~ResourceCache();

	friend class App;
};

