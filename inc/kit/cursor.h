#pragma once

#include <kit/vector.h>

namespace kit
{
	namespace gui
	{
		class Widget;
	}

	class Cursor
	{
	public:
		// Returns true if the cursor position is valid.
		bool validity () const;

		// Gets the cursor position relative to the window.
		Vector2i position () const;

		// Makes the cursor consumed for the duration of handling a given event.
		void consume ();

	private:
		//void _setValidity (bool);
		//bool _isConsumed () const;
		//void _resetConsumed ();
		//void _consume ();
		//void _setPosition (Vector2i);

		bool _consumed; // has been consumed by a widget for a particular event. resets every event.
		bool _validity; // is within window and is visible
		Vector2i _position; // relative to window position

		friend class Window;
	};
}

