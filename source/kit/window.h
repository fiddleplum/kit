#pragma once

#include "cursor.h"
#include "gui_container.h"
#include <vector>

typedef struct SDL_Window SDL_Window;
typedef void * SDL_GLContext;

class Window
{
public:
	// Constructor.
	Window(char const * title);

	// Destructor.
	~Window();

	// Sets the title of the window.
	void setTitle(char const * title);

	// Sets the window to windowed mode.
	void setWindowed();

	// Sets the window to fullscreen on a given display and resolution.
	void setFullscreen(int display, Vector2i size);

	// Sets the window to fullscreen on the display the window is within at its desktop resolution.
	void setFullscreen();

	// Returns the size of the window, exluding the borders and title bar.
	Vector2i getSize() const;

	// Returns true if the window is fullscreen.
	bool isFullscreen() const;

	// Returns the display that the window is within, determined by its center.
	int getDisplay() const;

	// Returns the cursor information associated with this window.
	Ptr<Cursor> getCursor() const;

	// Returns the currently set gui container.
	template <typename Type> Ptr<GuiContainer> getContainer() const;

	// Creates and sets the gui container of type Type.
	template <typename Type> Ptr<GuiContainer> setContainer();

	// Handles an event. Called by app.
	void handleEvent(Event const & event);

	// Renders the window. Called by app.
	void render(SDL_GLContext sdlGlContext) const;

private:
	void setCursorPosition(Vector2i position);
	void setCursorWithinWindow(bool state);

	SDL_Window * sdlWindow;
	OwnPtr<Cursor> cursor;
	OwnPtr<GuiContainer> gui;

	friend class App;
};

// Returns the number of displays in the system.
int getNumDisplays();

// Returns the resolution of a display before the application started.
Vector2i getStartingResolution(int display);

// Returns the current resolution of a display.
Vector2i getCurrentResolution(int display);

// Returns a list of all of the resolutions a display can support.
std::vector<Vector2i> getAllResolutions(int display);

template <typename Type> Ptr<GuiContainer> Window::getContainer() const
{
	return gui.as<Type>();
}

template <typename Type> Ptr<GuiContainer> Window::setContainer()
{
	gui.create<Type>();
}

