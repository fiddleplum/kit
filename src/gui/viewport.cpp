#include "viewport_p.h"
#include "../app_p.h"
#include "../open_gl.h"
#include "../scene/camera_internal.h"
#include "../scene.h"

namespace kit
{
	namespace gui
	{
		ViewportP::ViewportP ()
		{
			bounds = Recti::zero();
		}

		Recti ViewportP::getBounds () const
		{
			return bounds;
		}

		void ViewportP::setPosition (Vector2i position)
		{
			bounds.setMinKeepSize(position);
		}

		void ViewportP::setMaxSize (Vector2i maxSize)
		{
			bounds.setSize(maxSize);
			if(camera.isValid())
			{
				if(maxSize[1] != 0)
				{
					camera->setAspectRatio((float)maxSize[0] / (float)maxSize[1]);
				}
			}
		}

		Ptr<scene::Camera> ViewportP::getCamera () const
		{
			return camera;
		}

		void ViewportP::setCamera (Ptr<scene::Camera> camera)
		{
			this->camera = camera.as<scene::CameraP>();
			if(bounds.getSize()[1] != 0)
			{
				this->camera->setAspectRatio((float)bounds.getSize()[0] / (float)bounds.getSize()[1]);
			}
		}

		Ptr<scene::Scene> ViewportP::getScene () const
		{
			return scene;
		}

		void ViewportP::setScene (Ptr<scene::Scene> scene)
		{
			this->scene = scene;
		}

		bool ViewportP::handleEvent (Event const & event, bool cursorIsValid)
		{
			if(cursorIsValid)
			{
				if(getBounds().containsEx(app()->getCursorPosition()))
				{
					cursorIsValid = false;
				}
			}

			if(scene.isValid())
			{
				scene->handleEvent(event);
			}
		}

		void ViewportP::render (Vector2i windowSize)
		{
			if(!camera.isValid() || !camera.isValid())
			{
				return;
			}

			glViewport(bounds.min[0], windowSize[1] - bounds.max[1], bounds.max[0] - bounds.min[0], bounds.max[1] - bounds.min[1]);

			//scene->render(camera);

			glViewport(0, 0, windowSize[0], windowSize[1]);
		}
	}
}

