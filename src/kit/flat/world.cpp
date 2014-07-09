#include "world.h"

namespace kit
{
	namespace flat
	{
		World::World(Ptr<scene::Scene> scene)
		{
			_scene = scene;
		}

		Ptr<Object> World::addObject()
		{
			return Ptr<Object>();
		}

		Ptr<Camera> World::addCamera()
		{
			return Ptr<Camera>();
		}
	}
}

