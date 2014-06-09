#pragma once

#include <kit/gui/widget.h>

namespace kit
{
	namespace scene
	{
		class Camera;
		class Scene;
	}

	namespace gui
	{
		class Viewport : virtual public Widget
		{
		public:
			// Get the attached camera.
			virtual Ptr<scene::Camera> getCamera () const = 0;

			// Attach a camera.
			virtual void setCamera (Ptr<scene::Camera>) = 0;

			// Get the attached scene.
			virtual Ptr<scene::Scene> getScene () const = 0;

			// Attach a scene.
			virtual void setScene (Ptr<scene::Scene>) = 0;
		};
	}
}

