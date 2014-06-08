#include "viewport_internal.h"
#include "../app_internal.h"
#include "../open_gl.h"
#include "../camera.h"
#include "../scene.h"

namespace kit
{
	namespace gui
	{
		ViewportInternal::ViewportInternal ()
		{
			bounds = Recti::zero();
		}

		Recti ViewportInternal::getBounds () const
		{
			return bounds;
		}

		void ViewportInternal::setPosition (Vector2i position)
		{
			bounds.setMinKeepSize(position);
		}

		void ViewportInternal::setMaxSize (Vector2i maxSize)
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

		Ptr<Camera> ViewportInternal::getCamera () const
		{
			return camera;
		}

		void ViewportInternal::setCamera (Ptr<Camera> newCamera)
		{
			camera = newCamera;
			if(bounds.getSize()[1] != 0)
			{
				camera->setAspectRatio((float)bounds.getSize()[0] / (float)bounds.getSize()[1]);
			}
		}

		Ptr<Scene> ViewportInternal::getScene () const
		{
			return scene;
		}

		void ViewportInternal::setScene (Ptr<Scene> newScene)
		{
			scene = newScene;
		}

		bool ViewportInternal::handleEvent (Event const & event, bool cursorIsValid)
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

		void ViewportInternal::render (Vector2i windowSize)
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

