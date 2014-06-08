#pragma once

#include <kit/gui/widget.h>

namespace kit
{
	class Camera;
	class Scene;

	namespace gui
	{
		class Viewport : virtual public Widget
		{
		public:
			// Get the attached camera.
			virtual Ptr<Camera> getCamera () const = 0;

			// Attach a camera.
			virtual void setCamera (Ptr<Camera>) = 0;

			// Get the attached scene.
			virtual Ptr<Scene> getScene () const = 0;

			// Attach a scene.
			virtual void setScene (Ptr<Scene>) = 0;
		};
	}
}

