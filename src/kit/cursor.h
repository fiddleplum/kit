#pragma once

#include "vector.h"

namespace kit
{
	class Cursor
	{
	public:
		// Constructor.
		Cursor ();

		// Gets the cursor position relative to the window.
		Vector2i getPosition () const;

		// Returns true if the cursor position is within the window, visible, and not yet consumed.
		bool isPositionValid () const;

		// Allows a widget to consume the cursor so that widgets behind will see it as consumed.
		void consume ();

		// Sets the position of the cursor. Called by window on mouse move events.
		void setPosition (Vector2i);

		// Sets whether the position of the cursor is within the window. Called by app.
		void setWithinWindow (bool);

		// Resets the consumed state. Called by window before the handling of every event.
		void resetConsumed ();

	private:
		bool _isConsumed; // has been consumed by a widget for a particular event. resets every event.
		bool _isWithinWindow; // is within window
		bool _isVisible; // is visible
		Vector2i _position; // relative to window position
	};
}

