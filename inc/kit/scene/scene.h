#pragma once

#include <kit/ptr.h>

namespace kit
{
	namespace scene
	{
		class Light;
		class Camera;
		class Object;
		class Controller;

		class Scene
		{
		public:
			//virtual Ptr<Light> addLight() = 0;

			//virtual void removeLight(Ptr<Light>) = 0;

			virtual Ptr<Camera> addCamera() = 0;

			virtual void removeCamera(Ptr<Camera> camera) = 0;

			virtual Ptr<Object> addObject() = 0;

			virtual void removeObject(Ptr<Object> object) = 0;

			virtual Ptr<Controller> addController() = 0;

			virtual void removeController(Ptr<Controller> controller) = 0;
		};
	}
}

