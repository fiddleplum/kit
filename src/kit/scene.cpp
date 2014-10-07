#include "scene.h"
#include "open_gl.h"
#include <vector>

Scene::Scene()
{
}

Ptr<SceneLight> Scene::addLight()
{
	return *_lights.insert(OwnPtr<SceneLight>::createNew());
}

void Scene::removeLight(Ptr<SceneLight> light)
{
	_lights.erase(light);
}

Ptr<SceneCamera> Scene::addCamera()
{
	return *_cameras.insert(OwnPtr<SceneCamera>::createNew());
}

void Scene::removeCamera(Ptr<SceneCamera> camera)
{
	_cameras.erase(camera);
}

Ptr<SceneObject> Scene::addObject()
{
	return *_objects.insert(OwnPtr<SceneObject>::createNew());
}

void Scene::removeObject(Ptr<SceneObject> object)
{
	_objects.erase(object);
}

void Scene::setEventHandler(std::function<void(Event const &)> eventHandler)
{
	_eventHandler = eventHandler;
}

void Scene::handleEvent(Event const & event)
{
	if(_eventHandler)
	{
		_eventHandler(event);
	}
}

void Scene::render(Ptr<SceneCamera> camera)
{
	// Set the OpenGL settings.
	glEnable(GL_DEPTH_TEST);

	// Check for sorting. Pull out the ones that need to be resorted, and put them back in the proper place.
	std::vector<OwnPtr<SceneObject>> objectsToInsert;
	for(auto it = _objects.begin(); it != _objects.end();)
	{
		OwnPtr<SceneObject> object = *it;
		if(object->getModel()->needsResorting())
		{
			objectsToInsert.push_back(object);
			it = _objects.erase(object);
		}
		else
		{
			it++;
		}
	}
	for(OwnPtr<SceneObject> object : objectsToInsert)
	{
		_objects.insert(object);
		object->getModel()->resortingDone();
	}

	// Prepare the lights.
	std::vector<Vector3f> lightPositions;
	std::vector<Vector3f> lightColors;
	for(Ptr<SceneLight> light : _lights)
	{
		lightPositions.push_back(camera->getWorldToCameraTransform().transform(light->getPosition(), 1));
		lightColors.push_back(light->getColor());
	}
	if(!_lights.empty())
	{
		while(lightPositions.size() < SceneModel::maxLights)
		{
			lightPositions.push_back(Vector3f(0, 0, 0));
			lightColors.push_back(Vector3f(0, 0, 0));
		}
	}

	// Do the render.
	for(Ptr<SceneObject> object : _objects)
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

