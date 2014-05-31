#pragma once

#include <kit/vector.h>
#include <kit/ptr.h>
#include <kit/gui/widget_container.h>
#include <memory>
#include <vector>

namespace kit
{
	class Texture;
	class Shader;

	class Window : public gui::WidgetContainer
	{
	public:
		// Constructor.
		Window (char const * title);

		// Default destructor.
		~Window ();

		// Sets the title of the window.
		void setTitle (char const * title);

		// Sets the window to windowed mode.
		void setWindowed ();

		// Sets the window to fullscreen on a given display and resolution.
		void setFullscreen (int display, Vector2i size);

		// Sets the window to fullscreen on the display the window is within at its desktop resolution.
		void setFullscreen ();

		// Returns the size of the window, exluding the borders and title bar.
		Vector2i getSize () const;

		// Returns true if the window is fullscreen.
		bool isFullscreen () const;

		// Returns the display that the window is within, determined by its center.
		int getDisplay () const;

	private:
		class Data;
		OwnPtr<Data> data;
	};

	typedef Ptr<Window> WindowPtr;

	// Adds a new window.
	WindowPtr addWindow (char const * title);

	// Removes a window.
	void removeWindow (WindowPtr window);

	// Returns the number of displays in the system.
	int getNumDisplays ();

	// Returns the resolution of a display before the application started.
	Vector2i getStartingResolution (int display);

	// Returns the current resolution of a display.
	Vector2i getCurrentResolution (int display);

	// Returns a list of all of the resolutions a display can support.
	std::vector<Vector2i> getAllResolutions (int display);
};

