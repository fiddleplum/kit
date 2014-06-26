#pragma once

#include "gui/widget_container_p.h"
#include <kit/window.h>

struct SDL_Window;
typedef void * SDL_GLContext;

namespace kit
{
	class WindowP : public Window, public gui::WidgetContainerP
	{
	public:
		WindowP (char const * title);

		~WindowP ();

		void setTitle (char const * title) override;

		void setWindowed () override;

		void setFullscreen (int display, Vector2i size) override;

		void setFullscreen () override;

		Vector2i getSize () const override;

		bool isFullscreen () const override;

		int getDisplay () const override;

		void handleEvent (Event const & event);

		void render (SDL_GLContext sdlGlContext);

		SDL_Window * getSDLWindow () const;

		bool cursorIsValid () const override;

		Vector2i getCursorPosition () const override;

		void setCursorValidity (bool);

		bool cursorIsConsumed () const;

		void consumeCursor ();

		void setCursorPosition (Vector2i);

	private:
		SDL_Window * _sdlWindow;
		bool _cursorIsConsumed;
		bool _cursorIsValid;
		Vector2i _cursorPosition;
	};
}

