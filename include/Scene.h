#pragma once

#include "ResourceManager.h"
#include <set>

class Camera;
class Model;
class Texture;
class Shader;

class Scene
{
public:
	Scene();

	~Scene();

	Model * createModel();

	void destroyModel(Model * model);

	Camera * createCamera();

	void destroyCamera(Camera * camera);

	ResourceManager<Texture> & getTextureManager();

	ResourceManager<Shader> & getShaderManager();

	// Called by Camera::render.
	void render(Camera * camera);

private:
	Scene(Scene const &);
	Scene & operator =(Scene const &);

	class ModelCompare
	{
	public:
		bool operator ()(Model const * model0, Model const * model1);
	};

	std::set<Camera *> mCameras;
	std::set<Model *, ModelCompare> mModels;
	ResourceManager<Texture> mTextureManager;
	ResourceManager<Shader> mShaderManager;
};

