#pragma once

#include "gui/widget_container_p.h"
#include "cursor_p.h"
#include <kit/window.h>
#include <kit/platform.h>

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

		Ptr<Cursor> getCursor () const;

		void handleEvent (Event const & event);

		void render (SDL_GLContext sdlGlContext);

		SDL_Window * getSDLWindow () const;

	private:
		SDL_Window * _sdlWindow;
		OwnPtr<CursorP> _cursor;
	};
}

