#include "scene.h"
#include "open_gl.h"
#include <vector>

Scene::Scene()
{
}

Ptr<SceneLight> Scene::addLight()
{
	return *lights.insert(OwnPtr<SceneLight>::createNew());
}

void Scene::removeLight(Ptr<SceneLight> light)
{
	lights.erase(light);
}

Ptr<SceneCamera> Scene::addCamera()
{
	return *cameras.insert(OwnPtr<SceneCamera>::createNew());
}

void Scene::removeCamera(Ptr<SceneCamera> camera)
{
	cameras.erase(camera);
}

Ptr<SceneObject> Scene::addObject()
{
	return *objects.insert(OwnPtr<SceneObject>::createNew());
}

void Scene::removeObject(Ptr<SceneObject> object)
{
	objects.erase(object);
}

void Scene::setEventHandler(std::function<void(Event const &)> eventHandler)
{
	this->eventHandler = eventHandler;
}

void Scene::setUpdateHandler(std::function<void(float)> updateHandler)
{
	this->updateHandler = updateHandler;
}

void Scene::setPreRenderUpdateHandler(std::function<void()> preRenderUpdateHandler)
{
	this->preRenderUpdateHandler = preRenderUpdateHandler;
}

void Scene::handleEvent(Event const & event)
{
	if(eventHandler)
	{
		eventHandler(event);
	}
}

void Scene::update(float dt)
{
	if(updateHandler)
	{
		updateHandler(dt);
	}
}

void Scene::preRenderUpdate()
{
	if(preRenderUpdateHandler)
	{
		preRenderUpdateHandler();
	}
}

void Scene::render(Ptr<SceneCamera> camera)
{
	// Set the OpenGL settings.
	glEnable(GL_DEPTH_TEST);

	// Check for sorting. Pull out the ones that need to be resorted, and put them back in the proper place.
	std::vector<OwnPtr<SceneObject>> objectsToInsert;
	for(auto it = objects.begin(); it != objects.end();)
	{
		OwnPtr<SceneObject> object = *it;
		if(object->getModel()->needsResorting())
		{
			objectsToInsert.push_back(object);
			it = objects.erase(object);
		}
		else
		{
			it++;
		}
	}
	for(OwnPtr<SceneObject> object : objectsToInsert)
	{
		objects.insert(object);
		object->getModel()->resortingDone();
	}

	// Prepare the lights.
	std::vector<Coord3f> lightPositions;
	std::vector<Coord3f> lightColors;
	for(Ptr<SceneLight> light : lights)
	{
		lightPositions.push_back(camera->getWorldToCameraTransform().transform(light->getPosition(), 1));
		lightColors.push_back(light->getColor());
	}
	if(!lights.empty())
	{
		while(lightPositions.size() < SceneModel::maxLights)
		{
			lightPositions.push_back({0, 0, 0});
			lightColors.push_back({0, 0, 0});
		}
	}

	// Do the render.
	for(Ptr<SceneObject> object : objects)
	{
		object->getModel()->render(camera->getCameraToNdcTransform(), camera->getWorldToCameraTransform() * object->getLocalToWorldTransform(), lightPositions, lightColors);
	}

	glDisable(GL_DEPTH_TEST);
}

bool Scene::ObjectCompare::operator () (OwnPtr<SceneObject> object0, OwnPtr<SceneObject> object1)
{
	Ptr<SceneModel> model0 = object0->getModel();
	Ptr<SceneModel> model1 = object1->getModel();
	if(model0.isValid() && model1.isValid())
	{
		return *object0->getModel() < *object1->getModel();
	}
	else
	{
		return object0 < object1;
	}
}

