#include "open_gl.h"
#include "window.h"
#include "display.h"
#include <string>
#include <algorithm>
#include <map>
#include <SDL.h>

Window::Window(std::string const & title)
{
	Coord2i initialSize = {800, 600};
	cursorPositionIsValid = false;
	sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initialSize[0], initialSize[1], SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if(sdlWindow == nullptr)
	{
		throw std::runtime_error("Failed to create the window.");
	}
	root.setNew();
	root->setSize(initialSize);
}

Window::~Window()
{
	root.setNull();
	SDL_DestroyWindow(sdlWindow);
}

void Window::setTitle(std::string const & title)
{
	SDL_SetWindowTitle(sdlWindow, title.c_str());
}

Ptr<GuiContainer> Window::getRoot()
{
	return root;
}

void Window::setWindowed()
{
	SDL_SetWindowFullscreen(sdlWindow, 0);
	SDL_EnableScreenSaver();
}

void Window::setFullscreen(int display, Coord2i size)
{
	try
	{
		SDL_DisplayMode mode;
		if(SDL_GetDesktopDisplayMode(display, &mode) < 0)
		{
			throw std::exception();
		}
		mode.w = size[0];
		mode.h = size[1];
		SDL_DisableScreenSaver();
		if(SDL_SetWindowDisplayMode(sdlWindow, &mode) < 0)
		{
			throw std::exception();
		}
		if(SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
		{
			throw std::exception();
		}
	}
	catch(...)
	{
		throw std::runtime_error("Could not set display " + std::to_string(display) + " to the resolution " + std::to_string(size[0]) + "x" + std::to_string(size[1]) + ".");
	}
}

void Window::setFullscreen()
{
	int display = getDisplay();
	setFullscreen(display, Display::getStartingResolution(display));
}

Coord2i Window::getSize() const
{
	Coord2i size;
	SDL_GetWindowSize(sdlWindow, &size[0], &size[1]);
	return size;
}

bool Window::isFullscreen() const
{
	return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
}

int Window::getDisplay() const
{
	int display = SDL_GetWindowDisplayIndex(sdlWindow);
	if(display >= 0)
	{
		return display;
	}
	throw std::runtime_error("Could not get the display the window is within. ");
}

void Window::handleResize(Coord2i size)
{
	root->setSize(size);
}

void Window::handleEvent(Event const & event)
{
	root->handleEvent(event, cursorPosition, cursorPositionIsValid);
}

void Window::update(float dt)
{
	root->update(dt);
}

void Window::preRenderUpdate()
{
	root->preRenderUpdate();
}

void Window::render(SDL_GLContext glContext) const
{
	SDL_GL_MakeCurrent(sdlWindow, glContext);

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 1);
	glClearDepth(-1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Coord2i windowSize = getSize();
	glViewport(0, 0, windowSize[0], windowSize[1]);

	if(root)
	{
		root->render(windowSize);
	}

	SDL_GL_SwapWindow(sdlWindow);
}

void Window::setCursorPosition(Coord2i position)
{
	cursorPosition = position;
}

void Window::setCursorWithinWindow(bool state)
{
	cursorPositionIsValid = state;
}

SDL_Window * Window::getSDLWindow() const
{
	return sdlWindow;
}

