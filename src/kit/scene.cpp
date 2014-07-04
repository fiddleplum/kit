#include "scene.h"
#include "open_gl.h"

namespace kit
{
	Scene::Scene()
	{
		_paused = false;
	}

	Ptr<Light> Scene::addLight()
	{
		return *_lights.insert(OwnPtr<Light>(new Light));
	}

	void Scene::removeLight(Ptr<Light> light)
	{
		_lights.erase(light);
	}

	Ptr<Camera> Scene::addCamera()
	{
		return *_cameras.insert(OwnPtr<Camera>(new Camera));
	}

	void Scene::removeCamera(Ptr<Camera> camera)
	{
		_cameras.erase(camera.as<Camera>());
	}

	Ptr<Object> Scene::addObject()
	{
		return *_objects.insert(OwnPtr<Object>(new Object));
	}

	void Scene::removeObject(Ptr<Object> object)
	{
		_objects.erase(object);
	}

	Ptr<Controller> Scene::addController()
	{
		return Ptr<Controller>(); // TODO
		//return *_controllers.insert(OwnPtr<Controller>(new Controller));
	}

	void Scene::removeController(Ptr<Controller> controller)
	{
		_controllers.erase(controller);
	}

	void Scene::setPaused (bool paused)
	{
		_paused = paused;
	}

	void Scene::handleEvent(Event const & event)
	{
		for(auto controller : _controllers)
		{
			controller->handleEvent(event);
		}
	}

	void Scene::render(Ptr<Camera> camera)
	{
		// Set the OpenGL settings.
		glEnable(GL_DEPTH_TEST);

		// Check for sorting. Pull out the ones that need to be resorted, and put them back in the proper place.
		std::vector<OwnPtr<Object>> objectsToInsert;
		for(auto it = _objects.begin(); it != _objects.end();)
		{
			OwnPtr<Object> object = *it;
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
		for(OwnPtr<Object> object : objectsToInsert)
		{
			_objects.insert(object);
			object->getModel()->resortingDone();
		}

		// Prepare the lights.
		std::vector<Vector3f> lightPositions;
		std::vector<Vector3f> lightColors;
		for(Ptr<Light> light : _lights)
		{
			lightPositions.push_back(camera->getWorldToCameraTransform().transform(light->getPosition(), 1));
			lightColors.push_back(light->getColor());
		}
		if(!_lights.empty())
		{
			while(lightPositions.size() < Model::maxLights)
			{
				lightPositions.push_back(Vector3f(0, 0, 0));
				lightColors.push_back(Vector3f(0, 0, 0));
			}
		}

		// Do the render.
		for(Ptr<Object> object : _objects)
		{
			object->getModel()->render(camera, object, lightPositions, lightColors);
		}
	}

	bool Scene::ObjectCompare::operator () (OwnPtr<Object> object0, OwnPtr<Object> object1)
	{
		return *object0->getModel() < *object1->getModel();
	}
}
