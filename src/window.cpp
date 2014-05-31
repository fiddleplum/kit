#include <kit/window.h>
#include "texture.h"
#include "shader.h"
#include "open_gl.h"
#include "../external/SDL2-2.0.0/include/SDL.h"
#include <string>
#include <algorithm>
#include <map>

namespace kit
{
	extern std::map<Ptr<Window>, OwnPtr<Window>> windows;
	SDL_GLContext sdlGlContext = nullptr;

	class Window::Data
	{
	public:
		SDL_Window * sdlWindow;
	};

	Window::Window (char const * title)
	{
		data.set(new Data);
		data->sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if(data->sdlWindow == nullptr)
		{
			throw std::runtime_error("Failed to create the window. ");
		}
		if(sdlGlContext == nullptr)
		{
			sdlGlContext = SDL_GL_CreateContext(data->sdlWindow);
			glInitialize();
		}
	}

	Window::~Window ()
	{
		if(windows.empty())
		{
			SDL_GL_DeleteContext(sdlGlContext);
		}
		SDL_DestroyWindow(data->sdlWindow);
	}

	void Window::setTitle (char const * title)
	{
		SDL_SetWindowTitle(data->sdlWindow, title);
	}

	void Window::setWindowed ()
	{
		SDL_SetWindowFullscreen(data->sdlWindow, 0);
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
			if(SDL_SetWindowDisplayMode(data->sdlWindow, &mode) < 0)
			{
				throw std::exception();
			}
			if(SDL_SetWindowFullscreen(data->sdlWindow, SDL_WINDOW_FULLSCREEN) < 0)
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
		SDL_GetWindowSize(data->sdlWindow, &size[0], &size[1]);
		return size;
	}

	bool Window::isFullscreen () const
	{
		return (SDL_GetWindowFlags(data->sdlWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
	}

	int Window::getDisplay() const
	{
		int display = SDL_GetWindowDisplayIndex(data->sdlWindow);
		if(display >= 0)
		{
			return display;
		}
		throw std::runtime_error("Could not get the display the window is within. ");
	}

	WindowPtr addWindow (char const * title)
	{
		OwnPtr<Window> window (new Window (title));
		windows[window] = window;
		return window;
	}

	void removeWindow (WindowPtr window)
	{
		auto it = windows.find(window);
		if(it != windows.end())
		{
			OwnPtr<Window> window = it->second;
			windows.erase(window);
			window.setNull();
		}
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

