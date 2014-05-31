#pragma once

#include <kit/window.h>
#include "../external/SDL2-2.0.0/include/SDL.h"

namespace kit
{
	class WindowImpl : public Window
	{
	public:
		WindowImpl (char const * title);

		~WindowImpl ();

		void setTitle (char const * title);

		void setWindowed ();

		void setFullscreen (int display, Vector2i size);

		void setFullscreen ();

		Vector2i getSize () const;

		bool isFullscreen () const;

		int getDisplay () const;

		SDL_Window * getSDLWindow () const;

	private:
		SDL_Window * sdlWindow;
	};
}

