#pragma once

#include <kit/vector.h>
#include <kit/rect.h>
#include <kit/event.h>

namespace kit
{
	namespace gui
	{
		class Widget
		{
		public:
			Widget() {} // Empty default constructor.
			virtual ~Widget() {} // Empty virtual destructor for subclasses.

			virtual Recti getBounds () const = 0; // Gets the bounds of the widget.
			virtual void setPosition (Vector2i position) = 0; // Sets the position of the widget.
			virtual void setMaxSize (Vector2i maxSize) = 0; // Sets the maximum size this widget can be. The widget can max out to this size (text box), or not change its size at all (check box).

			virtual bool handleEvent (Event const & event, bool cursorIsValid) = 0; // Handles events. Returns true if the cursor is still valid.
			virtual void render (Vector2i windowSize) = 0; // Renders the widget.

		private:
			// TODO: Make these public and with the delete specifier, once MSVC upgrades.
			/// Not copy-constructable.
			Widget (Widget const &);

			/// Not copyable.
			Widget & operator = (Widget const &);
		};
	}
}

