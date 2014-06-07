#pragma once

#include <kit/gui/widget.h>
#include <kit/event.h>
#include <kit/vector.h>

namespace kit
{
	namespace gui
	{
		class WidgetInternal : virtual public Widget
		{
		public:
			// Handles events. Returns true if the cursor is still valid. Called by WidgetContainer.
			virtual bool handleEvent (Event const & event, bool cursorIsValid) = 0;

			// Renders the widget. Called by WidgetContainer.
			virtual void render (Vector2i windowSize) = 0;
		};
	}
}

