#pragma once

#include "../scene.h"
#include "camera.h"
#include "object.h"

namespace kit
{
	namespace flat
	{
		class World
		{
		public:
			World(Ptr<Scene> scene);

			Ptr<Camera> addCamera();

			Ptr<Object> addObject();

		private:
			Ptr<Scene> _scene;
		};
	}
}

