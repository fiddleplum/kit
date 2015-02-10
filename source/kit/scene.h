#pragma once

#include "scene_object.h"
#include "scene_light.h"
#include "scene_camera.h"
#include "event.h"
#include "ptr_set.h"
#include <functional>
#include <set>

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

	void setUpdateHandler(std::function<void(float)> updateHandler);

	void setPreRenderUpdateHandler(std::function<void()> preRenderUpdateHandler);

	// Called by app to handle an event.
	void handleEvent(Event const & event);

	// Called by App to update the scene every frame.
	void update(float dt);

	// Called by App to update the scene after a regular update but before the render. Good for things that keep track of other scene elements.
	void preRenderUpdate();

	// Called by GuiViewport to render the scene.
	void render(Ptr<SceneCamera> camera);

private:
	class ObjectCompare
	{
	public:
		bool operator () (OwnPtr<SceneObject> object0, OwnPtr<SceneObject> object1);
	};

	PtrSet<SceneLight> lights;
	PtrSet<SceneCamera> cameras;
	PtrSet<SceneObject, ObjectCompare> objects;
	std::function<void(Event const &)> eventHandler;
	std::function<void(float)> updateHandler;
	std::function<void()> preRenderUpdateHandler;
};

