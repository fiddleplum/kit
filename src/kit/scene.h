#pragma once

#include "object.h"
#include "controller.h"
#include "ptr_set.h"

namespace kit
{
	class Scene
	{
	public:
		Scene();

		Ptr<Light> addLight();

		void removeLight(Ptr<Light>);

		Ptr<Camera> addCamera();

		void removeCamera(Ptr<Camera> camera);

		Ptr<Object> addObject();

		void removeObject(Ptr<Object> object);

		Ptr<Controller> addController();

		void removeController(Ptr<Controller> controller);

		void setPaused (bool paused);

		void handleEvent(Event const & event);

		void render(Ptr<Camera> camera);

	private:
		class ObjectCompare
		{
		public:
			bool operator () (OwnPtr<Object> object0, OwnPtr<Object> object1);
		};

		PtrSet<Light> _lights;
		PtrSet<Camera> _cameras;
		PtrSet<Object, ObjectCompare> _objects;
		PtrSet<Controller> _controllers;
		bool _paused;
	};
}

