#pragma once

#include "gui_container.h"
#include <vector>

class Window : public virtual GuiContainer
{
public:
	// Virtual destructor, so it can be deleted properly.
	virtual ~Window() {}

	// Sets the title of the window.
	virtual void setTitle(char const * title) = 0;

	// Sets the window to windowed mode.
	virtual void setWindowed() = 0;

	// Sets the window to fullscreen on a given display and resolution.
	virtual void setFullscreen(int display, Coord2i size) = 0;

	// Sets the window to fullscreen on the display the window is within at its desktop resolution.
	virtual void setFullscreen() = 0;

	// Returns the size of the window, excluding the borders and title bar.
	virtual Coord2i getSize() const = 0;

	// Returns true if the window is fullscreen.
	virtual bool isFullscreen() const = 0;

	// Returns the display that the window is within, determined by its center.
	virtual int getDisplay() const = 0;
};

// These go in App

// Returns the number of displays in the system.
int getNumDisplays();

// Returns the resolution of a display before the application started.
Coord2i getStartingResolution(int display);

// Returns the current resolution of a display.
Coord2i getCurrentResolution(int display);

// Returns a list of all of the resolutions a display can support.
std::vector<Coord2i> getAllResolutions(int display);

