#include "app_p.h"
#include "open_gl.h"
#include "window_p.h"
#include "resources_p.h"
#include "../external/SDL2-2.0.0/include/SDL.h"

namespace kit
{
	OwnPtr<AppP> gApp;

	AppP::AppP ()
	{
		looping = false;
		cursorIsValid = false;
		firstMouseMoveEvent = true;
		sdlGlContext = nullptr;

		// Start SDL.
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) == -1)
		{
			throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
		}
		resources.set(new ResourcesP);
	}

	AppP::~AppP ()
	{
		resources.setNull();
		SDL_Quit();
	}

	void AppP::quit ()
	{
		looping = false;
	}

	float AppP::getTime() const
	{
		return SDL_GetTicks() / 1000.0f;
	}

	Vector2i AppP::getCursorPosition() const
	{
		Vector2i position;
		if(cursorIsValid)
		{
			SDL_GetMouseState(&position[0], &position[1]);
		}
		else
		{
			position.set(0, 0);
		}
		return position;
	}

	Ptr<kit::Window> AppP::addWindow (char const * title)
	{
		OwnPtr<WindowP> window (new WindowP (title));
		if(windows.empty())
		{
			sdlGlContext = SDL_GL_CreateContext(window->getSDLWindow());
			glInitialize();
		}
		auto windowIterator = windows.insert(windows.end(), window);
		windowMapping[window] = windowIterator;
		return window;
	}

	void AppP::removeWindow (Ptr<kit::Window> window)
	{
		auto it = windowMapping.find(window);
		if(it != windowMapping.end())
		{
			auto windowIt = it->second;
			windowMapping.erase(it);
			windows.erase(windowIt);
			if(windows.empty())
			{
				SDL_GL_DeleteContext(sdlGlContext);
			}
		}
	}

	void AppP::loop ()
	{
		// Make sure all of the windows have their sizes updated.
		for(auto window : windows)
		{
			window->callUpdateWidgetBoundsFunction();
		}

		looping = true;
		while(looping)
		{
			// Get SDL Events
			SDL_Event sdlEvent;
			while(looping && SDL_PollEvent(&sdlEvent))
			{
				handleSDLEvent(sdlEvent);
			}

			// Update
			for(auto window : windows)
			{
				if(looping)
				{
					window->handleEvent(UpdateEvent(), cursorIsValid);
				}
			}

			// Render
			for(auto window : windows)
			{
				if(looping)
				{
					window->render(sdlGlContext);
				}
			}
		}
	}

	void AppP::setCursorValid (bool valid)
	{
		cursorIsValid = valid;
	}

	Ptr<ResourcesP> AppP::getResources ()
	{
		return resources;
	}

	Ptr<WindowP> AppP::getWindowFromId (unsigned int id)
	{
		SDL_Window * sdlWindow = SDL_GetWindowFromID(id);
		if(sdlWindow != NULL)
		{
			for(auto window : windows)
			{
				if(sdlWindow == window->getSDLWindow())
				{
					return window;
				}
			}
		}
		return Ptr<WindowP> ();
	}

	KeyboardEvent::Key AppP::sdlKeyToKey (int sdlKey)
	{
		if(SDLK_a <= sdlKey && sdlKey <= SDLK_z)
		{
			return (KeyboardEvent::Key)((int)KeyboardEvent::A + (sdlKey - SDLK_a));
		}
		else if(SDLK_0 <= sdlKey && sdlKey <= SDLK_9)
		{
			return (KeyboardEvent::Key)((int)KeyboardEvent::N0 + (sdlKey - SDLK_0));
		}
		else if(SDLK_KP_0 <= sdlKey && sdlKey <= SDLK_KP_9)
		{
			return (KeyboardEvent::Key)((int)KeyboardEvent::Keypad0 + (sdlKey - SDLK_KP_0));
		}
		else if(SDLK_F1 <= sdlKey && sdlKey <= SDLK_F15)
		{
			return (KeyboardEvent::Key)((int)KeyboardEvent::F1 + (sdlKey - SDLK_F1));
		}
		switch(sdlKey)
		{
			case SDLK_UP:
				return KeyboardEvent::Up;
			case SDLK_DOWN:
				return KeyboardEvent::Down;
			case SDLK_LEFT:
				return KeyboardEvent::Left;
			case SDLK_RIGHT:
				return KeyboardEvent::Right;
			case SDLK_PAGEUP:
				return KeyboardEvent::PageUp;
			case SDLK_PAGEDOWN:
				return KeyboardEvent::PageDown;
			case SDLK_HOME:
				return KeyboardEvent::Home;
			case SDLK_END:
				return KeyboardEvent::End;
			case SDLK_RETURN:
				return KeyboardEvent::Enter;
			case SDLK_BACKSPACE:
				return KeyboardEvent::Backspace;
			case SDLK_INSERT:
				return KeyboardEvent::Insert;
			case SDLK_DELETE:
				return KeyboardEvent::Delete;
			case SDLK_TAB:
				return KeyboardEvent::Tab;
			case SDLK_SPACE:
				return KeyboardEvent::Space;
			case SDLK_ESCAPE:
				return KeyboardEvent::Escape;
			case SDLK_KP_PLUS:
				return KeyboardEvent::KeypadAdd;
			case SDLK_KP_MINUS:
				return KeyboardEvent::KeypadSubtract;
			case SDLK_KP_MULTIPLY:
				return KeyboardEvent::KeypadMultiply;
			case SDLK_KP_DIVIDE:
				return KeyboardEvent::KeypadDivide;
			case SDLK_KP_PERIOD:
				return KeyboardEvent::KeypadDecimal;
			case SDLK_KP_ENTER:
				return KeyboardEvent::KeypadEnter;
			case SDLK_BACKQUOTE:
				return KeyboardEvent::Grave;
			case SDLK_QUOTE:
				return KeyboardEvent::Apostrophe;
			case SDLK_SEMICOLON:
				return KeyboardEvent::Semicolon;
			case SDLK_COMMA:
				return KeyboardEvent::Comma;
			case SDLK_PERIOD:
				return KeyboardEvent::Period;
			case SDLK_SLASH:
				return KeyboardEvent::Slash;
			case SDLK_LEFTBRACKET:
				return KeyboardEvent::LeftBracket;
			case SDLK_RIGHTBRACKET:
				return KeyboardEvent::RightBracket;
			case SDLK_BACKSLASH:
				return KeyboardEvent::Backslash;
			case SDLK_MINUS:
				return KeyboardEvent::Hyphen;
			case SDLK_EQUALS:
				return KeyboardEvent::Equals;
			case SDLK_CAPSLOCK:
				return KeyboardEvent::Capslock;
			case SDLK_LSHIFT:
				return KeyboardEvent::LeftShift;
			case SDLK_RSHIFT:
				return KeyboardEvent::RightShift;
			case SDLK_LCTRL:
				return KeyboardEvent::LeftControl;
			case SDLK_RCTRL:
				return KeyboardEvent::RightControl;
			case SDLK_LALT:
				return KeyboardEvent::LeftAlt;
			case SDLK_RALT:
				return KeyboardEvent::RightAlt;
			case SDLK_PAUSE:
				return KeyboardEvent::Pause;
		}
		return KeyboardEvent::Unknown;
	}

	void AppP::handleSDLEvent (SDL_Event const & sdlEvent)
	{
		Ptr<WindowP> window;
		switch(sdlEvent.type)
		{
		case SDL_QUIT:
			quit();
			break;
		case SDL_WINDOWEVENT:
			window = getWindowFromId(sdlEvent.window.windowID);
			if(!window.isValid())
			{
				return;
			}
			switch(sdlEvent.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
				removeWindow(window);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				window->setMaxSize(Vector2i(sdlEvent.window.data1, sdlEvent.window.data2));
				break;
			case SDL_WINDOWEVENT_LEAVE:
				cursorIsValid = false;
				break;
			case SDL_WINDOWEVENT_ENTER:
				cursorIsValid = true;
				break;
			}
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			window = getWindowFromId(sdlEvent.key.windowID);
			if(window.isValid() || sdlEvent.key.repeat > 0) // only record actual press and releases
			{
				KeyboardEvent event;
				event.key = sdlKeyToKey(sdlEvent.key.keysym.sym);
				event.pressed = (sdlEvent.type == SDL_KEYDOWN ? true : false);
				window->handleEvent(event, cursorIsValid);
			}
			break;
		case SDL_TEXTINPUT:
			window = getWindowFromId(sdlEvent.text.windowID);
			if(window.isValid())
			{
				TextEvent event;
				event.text = sdlEvent.text.text;
				window->handleEvent(event, cursorIsValid);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			window = getWindowFromId(sdlEvent.button.windowID);
			if(window.isValid())
			{
				MouseButtonEvent event;
				switch(sdlEvent.button.button)
				{
					case SDL_BUTTON_LEFT:
						event.button = MouseButtonEvent::Left; break;
					case SDL_BUTTON_MIDDLE:
						event.button = MouseButtonEvent::Middle; break;
					case SDL_BUTTON_RIGHT:
						event.button = MouseButtonEvent::Right; break;
					case SDL_BUTTON_X1:
						event.button = 3; break;
					case SDL_BUTTON_X2:
						event.button = 4; break;
				}
				event.pressed = (sdlEvent.type == SDL_MOUSEBUTTONDOWN);
				window->handleEvent(event, cursorIsValid);
			}
			break;
		case SDL_MOUSEMOTION: // generates value changes for both moving and stopping
			window = getWindowFromId(sdlEvent.motion.windowID);
			if(window.isValid())
			{
				if(!firstMouseMoveEvent) // the first mouse motion event always gives absolute values in its xrel and yrel, and so is invalid
				{
					MouseMoveEvent event;
					event.relative[0] = sdlEvent.motion.xrel;
					event.relative[1] = sdlEvent.motion.yrel;
					event.absolute[0] = sdlEvent.motion.x;
					event.absolute[1] = sdlEvent.motion.y;
					window->handleEvent(event, cursorIsValid);
				}
				else
				{
					firstMouseMoveEvent = false;
				}
			}
			break;
		case SDL_MOUSEWHEEL:
			window = getWindowFromId(sdlEvent.wheel.windowID);
			if(window.isValid())
			{
				MouseWheelEvent event;
				event.up = (sdlEvent.wheel.y > 0);
				window->handleEvent(event, cursorIsValid);
			}
			break;
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			{
				ControllerButtonEvent event;
				event.controller = sdlEvent.jbutton.which;
				event.button = sdlEvent.jbutton.button;
				event.pressed = (sdlEvent.type == SDL_JOYBUTTONDOWN);
				for(auto window : windows)
				{
					window->handleEvent(event, cursorIsValid);
				}
			}
			break;
		case SDL_JOYAXISMOTION:
			{
				ControllerAxisEvent event;
				event.controller = sdlEvent.jaxis.which;
				event.axis = sdlEvent.jaxis.axis;
				if(sdlEvent.jaxis.value > 0)
				{
					event.value = (float)sdlEvent.jaxis.value / 32767.0f;
				}
				else
				{
					event.value = (float)sdlEvent.jaxis.value / 32768.0f;
				}
				for(auto window : windows)
				{
					window->handleEvent(event, cursorIsValid);
				}
			}
			break;
		case SDL_JOYHATMOTION:
			{
				ControllerAxisEvent event;
				event.controller = sdlEvent.jhat.which;
				event.axis = SDL_JoystickNumAxes(joysticks[sdlEvent.jhat.which]) + sdlEvent.jhat.hat * 2 + 0;
				if((sdlEvent.jhat.value & SDL_HAT_LEFT) != 0)
				{
					event.value = -1;
				}
				else if((sdlEvent.jhat.value & SDL_HAT_RIGHT) != 0)
				{
					event.value = +1;
				}
				else
				{
					event.value = 0;
				}
				for(auto window : windows)
				{
					window->handleEvent(event, cursorIsValid);
				}
				event.axis++;
				if((sdlEvent.jhat.value & SDL_HAT_DOWN) != 0)
				{
					event.value = -1;
				}
				else if((sdlEvent.jhat.value & SDL_HAT_UP) != 0)
				{
					event.value = +1;
				}
				else
				{
					event.value = 0;
				}
				for(auto window : windows)
				{
					window->handleEvent(event, cursorIsValid);
				}
			}
			break;
		case SDL_JOYBALLMOTION:
			{
				ControllerBallEvent event;
				event.controller = sdlEvent.jball.which;
				event.ball = sdlEvent.jball.ball;
				event.offset[0] = sdlEvent.jball.xrel;
				event.offset[1] = sdlEvent.jball.yrel;
				for(auto window : windows)
				{
					window->handleEvent(event, cursorIsValid);
				}
			}
			break;
		}
	}

	Ptr<AppP> app ()
	{
		return gApp;
	}
}

