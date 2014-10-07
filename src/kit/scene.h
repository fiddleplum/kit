#pragma once

#include "scene_object.h"
#include "scene_light.h"
#include "scene_camera.h"
#include "ptr_set.h"
#include "event.h"
#include <functional>

class Scene
{
public:
	Scene();

	Ptr<SceneLight> addLight();

	void removeLight(Ptr<SceneLight>);

	Ptr<SceneCamera> addCamera();

	void removeCamera(Ptr<SceneCamera> camera);

	Ptr<SceneObject> addObject();

	void removeObject(Ptr<SceneObject> object);

	void setEventHandler(std::function<void(Event const &)> eventHandler);

	// Called by app
	void handleEvent(Event const & event);

	void render(Ptr<SceneCamera> camera);

private:
	class ObjectCompare
	{
	public:
		bool operator () (OwnPtr<SceneObject> object0, OwnPtr<SceneObject> object1);
	};

	PtrSet<SceneLight> _lights;
	PtrSet<SceneCamera> _cameras;
	PtrSet<SceneObject, ObjectCompare> _objects;
	std::function<void(Event const &)> _eventHandler;
};

