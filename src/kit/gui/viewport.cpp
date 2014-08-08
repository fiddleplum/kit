#include "../open_gl.h"
#include "../cursor.h"
#include "../window.h"
#include "viewport.h"

namespace kit
{
	namespace gui
	{
		Viewport::Viewport()
		{
		}

		Recti Viewport::getBounds() const
		{
			return _bounds;
		}

		void Viewport::setPosition(Vector2i position)
		{
			_bounds.setMinKeepSize(position);
		}

		void Viewport::setMaxSize(Vector2i maxSize)
		{
			_bounds.setSize(maxSize);
			if(_camera.isValid())
			{
				if(maxSize[1] != 0)
				{
					_camera->setAspectRatio((float)maxSize[0] / (float)maxSize[1]);
				}
			}
		}

		Ptr<scene::Camera> Viewport::getCamera() const
		{
			return _camera;
		}

		void Viewport::setCamera(Ptr<scene::Camera> camera)
		{
			_camera = camera;
			if(_bounds.getSize()[1] != 0)
			{
				_camera->setAspectRatio((float)_bounds.getSize()[0] / (float)_bounds.getSize()[1]);
			}
		}

		Ptr<scene::Scene> Viewport::getScene() const
		{
			return _scene;
		}

		void Viewport::setScene(Ptr<scene::Scene> scene)
		{
			_scene = scene;
		}

		void Viewport::handleEvent(Event const & event)
		{
			if(event.window.isValid())
			{
				Ptr<Cursor> cursor = event.window->getCursor();
				if(cursor->isPositionValid() && getBounds().containsEx(cursor->getPosition()))
				{
					cursor->consume();
				}
			}
		}

		void Viewport::render(Vector2i windowSize)
		{
			if(!_camera.isValid() || !_scene.isValid())
			{
				return;
			}

			glViewport(_bounds.min[0], windowSize[1] - _bounds.max[1], _bounds.max[0] - _bounds.min[0], _bounds.max[1] - _bounds.min[1]);

			_scene->render(_camera);

			glViewport(0, 0, windowSize[0], windowSize[1]);
		}
	}
}

