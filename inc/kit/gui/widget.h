#pragma once

#include <kit/vector.h>
#include <kit/rect.h>
#include <kit/noncopyable.h>

namespace kit
{
	namespace gui
	{
		class Widget : Noncopyable
		{
		public:
			// Gets the bounds of the widget.
			virtual Recti getBounds () const = 0;

			// Sets the position of the widget.
			virtual void setPosition (Vector2i position) = 0;

			// Sets the maximum size this widget can be. The widget can max out to this size (text box), or not change its size at all (check box).
			virtual void setMaxSize (Vector2i maxSize) = 0;
		};
	}
}

