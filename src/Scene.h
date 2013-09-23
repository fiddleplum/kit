#pragma once

#include <set>

class Event;
class Camera;
class Model;

class Scene
{
public:
	Scene();

	~Scene();

	Model * createModel();

	void destroyModel(Model * model);

	Camera * createCamera();

	void destroyCamera(Camera * camera);

	// Called my Camera.
	void handleEvent(Event const & event);

	// Called by Camera.
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
};

