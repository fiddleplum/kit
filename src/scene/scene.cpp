#include "scene_p.h"
#include "object_p.h"
//#include "light_p.h"
#include "camera_p.h"
//#include "controller_p.h"
#include "../open_gl.h"

#include <memory>

namespace kit
{
	namespace scene
	{
		SceneP::SceneP()
		{
		}

		//Ptr<Light> SceneP::addLight()
		//{
		//	Ptr<Light> light (new Light);
		//	lights.insert(light);
		//	return light;
		//}

		//void SceneP::removeLight(Ptr<Light> light)
		//{
		//	lights.erase(light);
		//}

		Ptr<Camera> SceneP::addCamera()
		{
			OwnPtr<CameraP> camera (new CameraP);
			_cameras.insert(camera);
			return camera;
		}

		void SceneP::removeCamera(Ptr<Camera> camera)
		{
			_cameras.erase(camera.as<CameraP>());
		}

		Ptr<Object> SceneP::addObject()
		{
			OwnPtr<ObjectP> object (new ObjectP);
			_objects.insert(object);
			return object;
		}

		void SceneP::removeObject(Ptr<Object> object)
		{
			_objects.erase(object);
		}

		Ptr<Controller> SceneP::addController()
		{
			controllers.insert(controller);
		}

		void SceneP::removeController(Ptr<Controller> controller)
		{
			controllers.erase(controller);
		}

		void SceneP::handleEvent(Event const & event)
		{
			for(auto controller : controllers)
			{
				controller->handleEvent(event);
			}
		}

		void SceneP::render(Ptr<Camera> camera)
		{
			// Set the OpenGL settings.
			glEnable(GL_DEPTH_TEST);

			// Check for sorting. Pull out the ones that need to be resorted, and put them back in the proper place.
			std::vector<Ptr<Model>> modelsToInsert;
			for(auto it = mModels.begin(); it != mModels.end();)
			{
				Ptr<Model> model = *it;
				if(model->needsResorting())
				{
					it = mModels.erase(it);
					modelsToInsert.push_back(model);
				}
				else
				{
					it++;
				}
			}
			for(Ptr<Model> model : modelsToInsert)
			{
				mModels.insert(model);
				model->resortingDone();
			}

			// Prepare the lights.
			std::vector<Vector3f> lightPositions;
			std::vector<Vector3f> lightColors;
			for(Ptr<Light> light : lights)
			{
				lightPositions.push_back(camera->getView().transform(light->getPosition(), 1));
				lightColors.push_back(light->getColor());
			}
			while(lightPositions.size() < Model::maxLights)
			{
				lightPositions.push_back(Vector3f::zero());
				lightColors.push_back(Vector3f::zero());
			}

			// Do the render.
			for(Ptr<Model> model : mModels)
			{
				model->render(camera, lightPositions, lightColors);
			}
		}

		bool SceneP::ModelCompare::operator ()(Ptr<Model> model0, Ptr<Model> model1)
		{
			return *model0 < *model1;
		}
	}
}

