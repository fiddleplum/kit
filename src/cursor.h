#pragma once

#include "vector.h"

namespace kit
{
	class Cursor
	{
	public:
		// Returns true if the cursor position is valid.
		bool isValid () const;

		// Gets the cursor position relative to the window.
		Vector2i getPosition () const;

		void setValidity (bool);

		bool isConsumed () const;

		void resetConsumed ();

		void consume ();

		void setPosition (Vector2i);

	private:
		bool _isConsumed; // has been consumed by a widget for a particular event. resets every event.
		bool _isValid; // is within window and is visible
		Vector2i _position; // relative to window position
	};
}

