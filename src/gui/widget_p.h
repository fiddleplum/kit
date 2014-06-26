#pragma once

#include <kit/gui/widget.h>
#include <kit/event.h>
#include <kit/vector.h>
#include <kit/ptr.h>

namespace kit
{
	namespace gui
	{
		class WidgetP : virtual public Widget
		{
		public:
			virtual void handleEvent (Event const & event) = 0;

			virtual void render (Vector2i windowSize) = 0;
		};
	}
}

