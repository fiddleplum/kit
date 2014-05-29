#pragma once

#include <kit/vector.h>
#include <kit/ptr.h>
#include <memory>
#include <vector>

namespace kit
{
	class IWindow
	{
	public:
		// Default destructor.
		virtual ~IWindow () {}

		// Sets the title of the window.
		virtual void setTitle (char const * title) = 0;

		// Sets the window to windowed mode.
		virtual void setWindowed () = 0;

		// Sets the window to fullscreen on a given display and resolution.
		virtual void setFullscreen (int display, Vector2i size) = 0;

		// Sets the window to fullscreen on the display the window is within at its desktop resolution.
		virtual void setFullscreen () = 0;

		// Returns the size of the window, exluding the borders and title bar.
		virtual Vector2i getSize () const = 0;

		// Returns true if the window is fullscreen.
		virtual bool isFullscreen () const = 0;

		// Returns the display that the window is within, determined by its center.
		virtual int getDisplay () const = 0;	
	};

	typedef Ptr<IWindow> Window;

	// Adds a new window.
	Window addWindow (char const * title);

	// Removes a window.
	void removeWindow (Window window);

	// Returns the number of displays in the system.
	int getNumDisplays ();

	// Returns the resolution of a display before the application started.
	Vector2i getStartingResolution (int display);

	// Returns the current resolution of a display.
	Vector2i getCurrentResolution (int display);

	// Returns a list of all of the resolutions a display can support.
	std::vector<Vector2i> getAllResolutions (int display);
};

