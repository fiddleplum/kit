#pragma once

#include "widget_internal.h"
#include <kit/gui/viewport.h>

namespace kit
{
	namespace gui
	{
		class ViewportInternal : virtual public Viewport, virtual public WidgetInternal
		{
		public:
			ViewportInternal ();

			Recti getBounds () const override;

			void setPosition (Vector2i position) override;

			void setMaxSize (Vector2i maxSize) override;

			Ptr<Camera> getCamera () const override;

			void setCamera (Ptr<Camera>) override;

			Ptr<Scene> getScene () const override;

			void setScene (Ptr<Scene>) override;

			bool handleEvent (Event const & event, bool cursorIsValid) override;

			void render (Vector2i windowSize) override;

		private:
			Recti bounds;
			Ptr<Camera> camera;
			Ptr<Scene> scene;
		};
	}
}

