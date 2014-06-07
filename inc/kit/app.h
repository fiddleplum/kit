#pragma once

#include <kit/ptr.h>
#include <kit/window.h>
#include <memory>

namespace kit
{
	class App
	{
	public:
		// Quits the application.
		virtual void quit () = 0;

		// Adds a new window.
		virtual Ptr<Window> addWindow (char const * title) = 0;

		// Removes a window.
		virtual void removeWindow (Ptr<Window> window) = 0;
	};
}

