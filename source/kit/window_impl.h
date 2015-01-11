#pragma once

#include "gui_container_impl.h"
#include <kit/window.h>
#include <vector>
#include <SDL.h>

class WindowImpl : public virtual Window, public virtual GuiContainerImpl
{
public:
	WindowImpl(char const * title);

	virtual ~WindowImpl();

	void setTitle(char const * title) override;

	void setWindowed() override;

	void setFullscreen(int display, Vector2i size) override;

	void setFullscreen() override;

	Vector2i getSize() const override;

	bool isFullscreen() const override;

	int getDisplay() const override;

	void handleEvent(Event const & event);

	void render(SDL_GLContext glContext) const;

	void setCursorPosition(Vector2i position);

	void setCursorWithinWindow(bool state);

private:
	SDL_Window * sdlWindow;
	Vector2i cursorPosition;
	bool cursorPositionIsValid;
};

