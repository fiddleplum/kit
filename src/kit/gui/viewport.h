#pragma once

#include "../camera.h"
#include "../scene.h"
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

			void setMaxSize (Vector2i maxSize) override;

			// Get the attached camera.
			Ptr<Camera> getCamera () const;

			// Attach a camera.
			void setCamera (Ptr<Camera>);

			// Get the attached scene.
			Ptr<Scene> getScene () const;

			// Attach a scene.
			void setScene (Ptr<Scene>);

			// Handles an event.
			void handleEvent (Event const & event) override;

			// Renders the viewport.
			void render (Vector2i windowSize) override;

		private:
			Recti _bounds;
			Ptr<Camera> _camera;
			Ptr<Scene> _scene;
		};
	}
}

