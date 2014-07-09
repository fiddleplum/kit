#pragma once

#include "object.h"
#include "light.h"
#include "camera.h"
#include "../ptr_set.h"
#include "../event.h"
#include <functional>

namespace kit
{
	namespace scene
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

			void setEventHandler(std::function<void(Event const &)> eventHandler);

			// Called by app
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
			std::function<void(Event const &)> _eventHandler;
		};
	}
}

