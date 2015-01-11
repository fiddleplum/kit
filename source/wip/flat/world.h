#pragma once

#include "../scene/scene.h"
#include "camera.h"
#include "object.h"

namespace kit
{
	namespace flat
	{
		class World
		{
		public:
			World(Ptr<scene::Scene> scene);

			Ptr<Camera> addCamera();

			Ptr<Object> addObject();

		private:
			Ptr<scene::Scene> _scene;
		};
	}
}

