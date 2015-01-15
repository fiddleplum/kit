#pragma once

#include "coord.h"
#include "event.h"
#include "ptr.h"
#include "gui_element.h"
#include <functional>
#include <vector>

// QUESTION:
/*
I'm working on how to set the size of the root element when it is just a guielement, not a container.
Should window contain a container or should it be a container or contain a guielement?
Should button inherit from sprite rather than contain it?
Should there be size events or just a setSize function, and should it be in guielement?

Option:
Make Window have a container element.
This means that layer must inherit from container to be a root element.
This means that container must have its functions protected.
This means that all other container-type elements must inherit rather than contain container.

Option:
Make all elements have a setSize function.
This means that things like button and sprite need to do nothing when it is called.

Option:
Make resizing an event.
This means that positioning should also be an event for consistency.

*/

struct SDL_Window;
union SDL_Event;
typedef void * SDL_GLContext;

class Window
{
public:
	Window(char const * title);

	virtual ~Window();

	void setTitle(char const * title);

	template <typename T> Ptr<T> setRootElement();

	void setWindowed();

	void setFullscreen(int display, Coord2i size);

	void setFullscreen();

	Coord2i getSize() const;

	bool isFullscreen() const;

	int getDisplay() const;

	void handleResize(Coord2i size);

	void handleEvent(Event const & event);

	void render(SDL_GLContext glContext) const;

	void setCursorPosition(Coord2i position);

	void setCursorWithinWindow(bool state);

	void setSizeChangedHandler(std::function<void(Coord2i)> handler);

private:
	SDL_Window * sdlWindow;
	OwnPtr<GuiElement> rootElement;
	Coord2i cursorPosition;
	bool cursorPositionIsValid;


};

template <typename T> Ptr<T> Window::setRootElement()
{
	rootElement.setNew<T>();
	return rootElement;
}

