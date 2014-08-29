#pragma once

#include "../scene/camera.h"
#include "../scene/scene.h"
#include "widget.h"

namespace kit
{
	namespace gui
	{
		class Viewport : public Widget
		{
		public:
			Viewport ();

			Recti getBounds () const override;

			void setPosition (Vector2i position) override;

			void setMaxCoord (Vector2i maxCoord) override;

			// Get the attached camera.
			Ptr<scene::Camera> getCamera () const;

			// Attach a camera.
			void setCamera (Ptr<scene::Camera>);

			// Get the attached scene.
			Ptr<scene::Scene> getScene () const;

			// Attach a scene.
			void setScene (Ptr<scene::Scene>);

			// Handles an event.
			void handleEvent (Event const & event) override;

			// Renders the viewport.
			void render (Vector2i windowSize) override;

		private:
			Recti _bounds;
			Ptr<scene::Camera> _camera;
			Ptr<scene::Scene> _scene;
		};
	}
}

