#pragma once

#include "widget_p.h"
//#include "../scene/camera.h"
#include <kit/gui/viewport.h>

namespace kit
{
	namespace gui
	{
		class ViewportP : virtual public Viewport, virtual public WidgetP
		{
		public:
			ViewportP ();

			Recti getBounds () const override;

			void setPosition (Vector2i position) override;

			void setMaxSize (Vector2i maxSize) override;

			Ptr<scene::Camera> getCamera () const override;

			void setCamera (Ptr<scene::Camera>) override;

			Ptr<scene::Scene> getScene () const override;

			void setScene (Ptr<scene::Scene>) override;

			void handleEvent (Event const & event) override;

			void render (Vector2i windowSize) override;

		private:
			Recti bounds;
			Ptr<scene::Camera> camera;
			Ptr<scene::Scene> scene;
		};
	}
}

