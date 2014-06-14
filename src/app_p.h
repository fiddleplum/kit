#pragma once

#include <kit/app.h>
#include <kit/event.h>
#include <list>
#include <map>
#include <vector>

typedef void * SDL_GLContext;
typedef struct _SDL_Joystick SDL_Joystick;
union SDL_Event;

namespace kit
{
	class Resources;
	class WindowP;

	class AppP : virtual public App
	{
	public:
		AppP ();

		~AppP ();

		void quit () override;

		Ptr<kit::Window> addWindow (char const * title) override;

		void removeWindow (Ptr<kit::Window> window) override;

		float getTime() const override;

		Vector2i getCursorPosition() const override;

		void loop ();

		void setCursorValid (bool valid);

		Ptr<Resources> getResources ();

	private:
		Ptr<WindowP> getWindowFromId (unsigned int id);
		KeyboardEvent::Key sdlKeyToKey (int sdlKey);
		void handleSDLEvent (SDL_Event const & sdlEvent);

		std::list<OwnPtr<WindowP>> windows;
		std::map<Ptr<kit::Window>, std::list<OwnPtr<WindowP>>::iterator> windowMapping;
		SDL_GLContext sdlGlContext;
		bool looping;
		bool cursorIsValid;
		bool firstMouseMoveEvent;
		std::vector<SDL_Joystick *> joysticks;
		OwnPtr<Resources> resources;
	};

	Ptr<AppP> app ();
}

