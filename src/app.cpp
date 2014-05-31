#include "app.h"
#include "window.h"
#include <kit/app.h>
#include <map>
#include "../external/SDL2-2.0.0/include/SDL.h"

namespace kit
{
	std::map<Ptr<Window>, OwnPtr<WindowImpl>> windows;
	bool looping = false;

	Ptr<WindowImpl> getWindowFromId (Uint32 id)
	{
		SDL_Window * sdlWindow = SDL_GetWindowFromID(id);
		if(sdlWindow != NULL)
		{
			for(auto it : windows)
			{
				Ptr<WindowImpl> window = it.second;
				if(sdlWindow == window->getSDLWindow())
				{
					return window;
				}
			}
		}
		return Ptr<WindowImpl> ();
	}

	void loop ()
	{
		looping = true;
		while(looping)
		{
			// Get SDL Events
			SDL_Event sdlEvent;
			Ptr<WindowImpl> window;
			while(looping && SDL_PollEvent(&sdlEvent))
			{
				switch(sdlEvent.type)
				{
				case SDL_QUIT:
					quit();
					break;
				case SDL_WINDOWEVENT:
					window = getWindowFromId(sdlEvent.window.windowID);
					if(!window.isValid())
					{
						continue;
					}
					switch(sdlEvent.window.event)
					{
					case SDL_WINDOWEVENT_CLOSE:
						removeWindow(window);
						break;
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						window->setMaxSize(Vector2i(sdlEvent.window.data1, sdlEvent.window.data2));
						break;
					//case SDL_WINDOWEVENT_MINIMIZED:
					//case SDL_WINDOWEVENT_FOCUS_LOST:
					//case SDL_WINDOWEVENT_LEAVE:
					//	setCursorPositionValid(false);
					//	break;
					//case SDL_WINDOWEVENT_FOCUS_GAINED:
					//case SDL_WINDOWEVENT_ENTER:
					//	setCursorPositionValid(true);
					//	break;
					}
					break;
				default:
					//handleSDLInputEvent(sdlEvent);
					break;
				}
			}

			// Update

			// Render
		}
	}

	void quit ()
	{
		looping = false;
	}
}

