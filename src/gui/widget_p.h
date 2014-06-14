#pragma once

#include <kit/gui/widget.h>
#include <kit/event.h>
#include <kit/vector.h>

namespace kit
{
	namespace gui
	{
		class WidgetP : virtual public Widget
		{
		public:
			virtual bool handleEvent (Event const & event, bool cursorIsValid) = 0;

			virtual void render (Vector2i windowSize) = 0;
		};
	}
}

