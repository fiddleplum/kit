#include "app.h"
#include "gui/widget.h"
#include "../external/SDL2-2.0.0/include/SDL.h"
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <iostream>

namespace kit
{
	namespace App
	{
		SDL_Window * window = nullptr;
		SDL_GLContext glContext = nullptr;
		bool gFirstMouseMoveEvent = true;
		std::vector<SDL_Joystick *> gJoysticks;
		bool gCursorPositionValid = true;

		void start(std::vector<std::string> const & params);

		void showMessage(std::string const & text)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", text.c_str(), window);
			log(text);
		}

		float getTime()
		{
			return SDL_GetTicks() / 1000.0f;
		}

		void setTitle(std::string const & title)
		{
			SDL_SetWindowTitle(window, title.c_str());
		}

		bool isFullscreen()
		{
			return (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
		}

		Vector2i getSize()
		{
			Vector2i size;
			SDL_GetWindowSize(window, &size[0], &size[1]);
			return size;
		}

		void setFullscreen()
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}

		void setWindowed(Vector2i size)
		{
			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowSize(window, size[0], size[1]);
		}

		void finishRender()
		{
			SDL_GL_SwapWindow(window);
		}

		KeyboardEvent::Key sdlKeyToKey(int sdlKey)
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
			throw std::exception();
		}

		void handleSDLInputEvent(SDL_Event const & sdlEvent)
		{
			if(sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP)
			{
				if(sdlEvent.key.repeat > 0)
				{
					return; // only record actual press and releases
				}
				KeyboardEvent event;
				try
				{
					event.key = sdlKeyToKey(sdlEvent.key.keysym.sym);
				}
				catch(...)
				{
					return; // do nothing with the unknown keypress
				}
				event.pressed = (sdlEvent.type == SDL_KEYDOWN ? true : false);
				handleEvent(event);
			}
			else if(sdlEvent.type == SDL_TEXTINPUT)
			{
				TextEvent event;
				event.text = sdlEvent.text.text;
			}
			else if(sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP)
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
				handleEvent(event);
			}
			else if(sdlEvent.type == SDL_MOUSEMOTION) // generate value changes for both moving and stopping
			{
				if(!gFirstMouseMoveEvent) // the first mouse motion event always gives absolute values in its xrel and yrel, and so is invalid
				{
					MouseMoveEvent event;
					event.relative[0] = sdlEvent.motion.xrel;
					event.relative[1] = sdlEvent.motion.yrel;
					event.absolute[0] = sdlEvent.motion.x;
					event.absolute[1] = sdlEvent.motion.y;
					handleEvent(event);
					gCursorPositionValid = true;
				}
				else
				{
					gFirstMouseMoveEvent = false;
				}
			}
			else if(sdlEvent.type == SDL_MOUSEWHEEL)
			{
				MouseWheelEvent event;
				event.up = (sdlEvent.wheel.y > 0);
				handleEvent(event);
			}
			else if(sdlEvent.type == SDL_JOYBUTTONDOWN || sdlEvent.type == SDL_JOYBUTTONUP)
			{
				ControllerButtonEvent event;
				event.controller = sdlEvent.jbutton.which;
				event.button = sdlEvent.jbutton.button;
				event.pressed = (sdlEvent.type == SDL_JOYBUTTONDOWN);
				handleEvent(event);
			}
			else if(sdlEvent.type == SDL_JOYAXISMOTION)
			{
				ControllerAxisEvent  event;
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
				handleEvent(event);
			}
			else if(sdlEvent.type == SDL_JOYHATMOTION)
			{
				ControllerAxisEvent event;
				event.controller = sdlEvent.jhat.which;
				event.axis = SDL_JoystickNumAxes(gJoysticks[sdlEvent.jhat.which]) + sdlEvent.jhat.hat * 2 + 0;
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
				handleEvent(event);
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
				handleEvent(event);
			}
			else if(sdlEvent.type == SDL_JOYBALLMOTION)
			{
				ControllerBallEvent event;
				event.controller = sdlEvent.jball.which;
				event.ball = sdlEvent.jball.ball;
				event.offset[0] = sdlEvent.jball.xrel;
				event.offset[1] = sdlEvent.jball.yrel;
				handleEvent(event);
			}
		}

		void startupInput()
		{
			refreshControllers();
		}

		void shutdownInput()
		{
			for(unsigned int i = 0; i < gJoysticks.size(); ++i)
			{
				SDL_JoystickClose(gJoysticks[i]);
			}
		}

		void setCursorPositionValid(bool valid)
		{
			gCursorPositionValid = valid;
		}

		int getNumControllers()
		{
			return (int)gJoysticks.size();
		}

		void refreshControllers()
		{
			if(SDL_WasInit(SDL_INIT_JOYSTICK) != 0)
			{
				for(int i = 0; i < (int)gJoysticks.size(); ++i)
				{
					SDL_JoystickClose(gJoysticks[i]);
				}
				SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
			}

			if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
			{
				throw std::runtime_error("Could not start SDL joystick subsystem.");
			}

			int nugJoysticks = SDL_NumJoysticks();
			for(int i = 0; i < nugJoysticks; ++i)
			{
				SDL_Joystick * joystick = SDL_JoystickOpen(i);
				if(joystick == NULL)
				{
					throw std::runtime_error("Could not initialize input controller " + std::to_string(i + 1));
				}
				gJoysticks.push_back(joystick);
			}
		}

		bool hasValidCursorPosition()
		{
			return gCursorPositionValid;
		}

		Vector2i getCursorPosition()
		{
			Vector2i position;
			SDL_GetMouseState(&position[0], &position[1]);
			return position;
		}

		bool isCursorVisible()
		{
			return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
		}

		void setCursorVisible (bool visible)
		{
			if(visible)
			{
				SDL_ShowCursor(SDL_ENABLE);
			}
			else
			{
				SDL_ShowCursor(SDL_DISABLE);
			}
		}

		bool isCursorEnabled()
		{
			return SDL_GetRelativeMouseMode() == SDL_TRUE ? true : false;
		}

		void setCursorEnabled(bool enabled)
		{
			int ret = SDL_SetRelativeMouseMode(enabled ? SDL_FALSE : SDL_TRUE);
			if(ret == -1)
			{
				throw std::runtime_error(std::string("Cursor cannot be disabled: ") + SDL_GetError() + ". ");
			}
		}

		void handleOSEvents()
		{
			// Get SDL Events
			std::vector<SDL_Event> sdlEvents;
			SDL_PumpEvents();
			do
			{
				SDL_Event sdlEvent;
				if(SDL_PollEvent(&sdlEvent) == 0)
				{
					break;
				}
				sdlEvents.push_back(sdlEvent);
			} while(true);

			// Process the SDL events.
			for(SDL_Event const & sdlEvent : sdlEvents)
			{
				if(sdlEvent.type == SDL_QUIT)
				{
					quit();
				}
				else if(sdlEvent.type == SDL_WINDOWEVENT)
				{
					switch(sdlEvent.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE:
							quit();
							break;
						case SDL_WINDOWEVENT_RESIZED:
						case SDL_WINDOWEVENT_MAXIMIZED:
						case SDL_WINDOWEVENT_RESTORED:
							{
								ResizeEvent resizeEvent;
								SDL_GetWindowSize(window, &resizeEvent.size[0], &resizeEvent.size[1]);
								handleEvent(resizeEvent);
							}
							break;
						case SDL_WINDOWEVENT_MINIMIZED:
						case SDL_WINDOWEVENT_FOCUS_LOST:
						case SDL_WINDOWEVENT_LEAVE:
							setCursorPositionValid(false);
							break;
						case SDL_WINDOWEVENT_FOCUS_GAINED:
						case SDL_WINDOWEVENT_ENTER:
							setCursorPositionValid(true);
							break;
					}
				}
				else
				{
					handleSDLInputEvent(sdlEvent);
				}
			}
		}
	}

	int main(int argc, char *argv[])
	{
		// Grab the params.
		std::vector<std::string> params;
		for(int i = 1; i < argc; ++i)
		{
			params.push_back(std::string(argv[i]));
		}
    
		// Initialize SDL.
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) == -1)
		{
			App::showMessage(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
			return -1;
		}

		// Create the window and renderer.
		App::window = SDL_CreateWindow("Untitled", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if(App::window == NULL)
		{
			App::showMessage(std::string("Failed to create the SDL window:  ") + SDL_GetError() + ". ");
			return -1;
		}
		App::glContext = SDL_GL_CreateContext(App::window);

		App::start(params);

		// Close window.
		App::shutdownInput();
		SDL_GL_DeleteContext(App::glContext);
		SDL_DestroyWindow(App::window);
		SDL_Quit();

		return 0;
	}
}

