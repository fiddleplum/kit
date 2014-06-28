#pragma once

#include <kit/scene/scene.h>
#include <kit/ptr_set.h>

namespace kit
{
	class Event;

	namespace scene
	{
		class CameraP;
		class ObjectP;
		class ControllerP;

		class SceneP : virtual public Scene, public Noncopyable
		{
		public:
			SceneP();

			//Ptr<Light> addLight() override;

			//void removeLight(Ptr<Light>) override;

			Ptr<Camera> addCamera() override;

			void removeCamera(Ptr<Camera> camera) override;

			Ptr<Object> addObject() override;

			void removeObject(Ptr<Object> object) override;

			Ptr<Controller> addController() override;

			void removeController(Ptr<Controller> controller) override;

			// Called by Camera.
			void handleEvent(Event const & event);

			// Called by Camera.
			void render(Ptr<Camera> camera);

		private:
			class ObjectCompare
			{
			public:
				bool operator ()(OwnPtr<ObjectP> object0, OwnPtr<ObjectP> object1);
			};

			//std::set<OwnPtr<Light>> lights;
			PtrSet<CameraP> _cameras;
			PtrSet<ObjectP, ObjectCompare> _objects;
			PtrSet<ControllerP> _controllers;
		};
	}
}

