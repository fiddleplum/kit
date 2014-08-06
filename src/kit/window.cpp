#include "open_gl.h"
#include "window.h"
#include <string>
#include <algorithm>
#include <map>
#include <SDL.h>

namespace kit
{
	Window::Window (char const * title)
	{
		Vector2i size (800, 600);

		_sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size[0], size[1], SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if(_sdlWindow == nullptr)
		{
			throw std::runtime_error("Failed to create the window. ");
		}
		setMaxSize(size);

		_cursor.setNew();
	}

	Window::~Window ()
	{
		SDL_DestroyWindow(_sdlWindow);
	}

	void Window::setTitle (char const * title)
	{
		SDL_SetWindowTitle(_sdlWindow, title);
	}

	void Window::setWindowed ()
	{
		SDL_SetWindowFullscreen(_sdlWindow, 0);
		SDL_EnableScreenSaver();
	}

	void Window::setFullscreen (int display, Vector2i size)
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
			if(SDL_SetWindowDisplayMode(_sdlWindow, &mode) < 0)
			{
				throw std::exception();
			}
			if(SDL_SetWindowFullscreen(_sdlWindow, SDL_WINDOW_FULLSCREEN) < 0)
			{
				throw std::exception();
			}
		}
		catch (...)
		{
			throw std::runtime_error("Could not set display " + std::to_string(display) + " to the resolution " + std::to_string(size[0]) + "x" + std::to_string(size[1]) + ". ");
		}
	}

	void Window::setFullscreen ()
	{
		setFullscreen(getDisplay(), getStartingResolution(getDisplay()));
	}

	Vector2i Window::getSize () const
	{
		Vector2i size;
		SDL_GetWindowSize(_sdlWindow, &size[0], &size[1]);
		return size;
	}

	bool Window::isFullscreen () const
	{
		return (SDL_GetWindowFlags(_sdlWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
	}

	int Window::getDisplay() const
	{
		int display = SDL_GetWindowDisplayIndex(_sdlWindow);
		if(display >= 0)
		{
			return display;
		}
		throw std::runtime_error("Could not get the display the window is within. ");
	}

	Ptr<Cursor> Window::getCursor () const
	{
		return _cursor;
	}

	void Window::handleEvent (Event const & event)
	{
		_cursor->resetConsumed();
		if(event.type == Event::MouseMove)
		{
			_cursor->setPosition(event.as<MouseMoveEvent>().absolute);
		}
		WidgetContainer::handleEvent(event);
	}

	void Window::render (SDL_GLContext sdlGlContext)
	{
		SDL_GL_MakeCurrent(_sdlWindow, sdlGlContext);

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

		Vector2i windowSize = getSize();
		glViewport(0, 0, windowSize[0], windowSize[1]);

		WidgetContainer::render(windowSize);

		SDL_GL_SwapWindow(_sdlWindow);
	}

	SDL_Window * Window::getSDLWindow () const
	{
		return _sdlWindow;
	}

	int getNumDisplays ()
	{
		int numDisplays = SDL_GetNumVideoDisplays();
		if(numDisplays < 0)
		{
			numDisplays = 0;
		}
		return numDisplays;
	}

	Vector2i getStartingResolution (int display)
	{
		SDL_DisplayMode mode;
		if(SDL_GetDesktopDisplayMode(display, &mode) == 0)
		{
			return Vector2i (mode.w, mode.h);
		}
		throw std::runtime_error("Could not get the initial resolution of display " + std::to_string(display) + ". ");
	}

	Vector2i getCurrentResolution (int display)
	{
		SDL_DisplayMode mode;
		if(SDL_GetCurrentDisplayMode(display, &mode) == 0)
		{
			return Vector2i (mode.w, mode.h);
		}
		throw std::runtime_error("Could not get the current resolution of display " + std::to_string(display) + ". ");
	}

	std::vector<Vector2i> getAllResolutions (int display)
	{
		std::vector<Vector2i> resolutions;

		SDL_DisplayMode desktopMode;
		if(SDL_GetDesktopDisplayMode(display, &desktopMode) == 0)
		{
			resolutions.push_back(Vector2i(desktopMode.w, desktopMode.h));
		}

		int numModes = SDL_GetNumDisplayModes(display);
		if(numModes < 0)
		{
			numModes = 0;
		}

		SDL_DisplayMode mode;
		for(int i = 0; i < numModes; i++)
		{
			if(SDL_GetDisplayMode(display, i, &mode) == 0 && mode.format == desktopMode.format && mode.refresh_rate == desktopMode.refresh_rate)
			{
				Vector2i resolution (mode.w, mode.h);
				for(auto it = resolutions.begin(); it != resolutions.end(); it++)
				{
					if(*it == resolution)
					{
						break;
					}
				}
				if(std::find(resolutions.begin(), resolutions.end(), resolution) == resolutions.end())
				{
					resolutions.push_back(resolution);
				}
			}
		}
		return resolutions;
	}
};

