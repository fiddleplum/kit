#include "app.h"
#include "open_gl.h"
#include "resources.h"
#include "log.h"
#include "audio.h"
#include "controllers.h"
#include "ptr_set.h"
#include <SDL.h>

namespace kit
{
	namespace controllers
	{
		extern int getNumSDLAxes(int joystick);
	}

	namespace app
	{
		Ptr<Window> getWindowFromId(unsigned int id);
		KeyboardEvent::Key sdlKeyToKey(int sdlKey);
		void handleSDLEvent(SDL_Event const & sdlEvent);

		PtrSet<Window> _windows;
		PtrSet<scene::Scene> _scenes;
		SDL_GLContext _sdlGlContext;
		bool _looping;
		bool _firstMouseMoveEvent;
		float targetFrameRate = 60.f;

		void handleEvent(Event const& event);
			
		void initialize()
		{
			_looping = false;
			_firstMouseMoveEvent = true;
			_sdlGlContext = nullptr;

			// Start SDL.
			if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) == -1)
			{
				throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
			}

			resources::initialize();
			controllers::initialize();
			audio::initialize();
		}

		void finalize()
		{
			audio::finalize();
			controllers::finalize();
			resources::finalize();
			SDL_Quit();
		}

		void quit()
		{
			_looping = false;
		}

		float getTime()
		{
			return SDL_GetTicks() / 1000.0f;
		}

		Ptr<Window> addWindow(char const * title)
		{
			OwnPtr<Window> window;
			window.create(title);
			if(_windows.empty())
			{
				_sdlGlContext = SDL_GL_CreateContext(window->getSDLWindow());
				glInitialize();
			}
			_windows.insert(window);
			return window;
		}

		void removeWindow(Ptr<Window> window)
		{
			_windows.erase(window);
			if(_windows.empty())
			{
				SDL_GL_DeleteContext(_sdlGlContext);
				_sdlGlContext = 0;
			}
		}

		Ptr<scene::Scene> addScene()
		{
			OwnPtr<scene::Scene> scene;
			scene.create();
			_scenes.insert(scene);
			return scene;
		}

		void removeScene(Ptr<scene::Scene> scene)
		{
			_scenes.erase(scene);
		}

		void loop()
		{
			// Make sure all of the windows have their sizes updated.
			for(auto window : _windows)
			{
				window->callUpdateWidgetBoundsFunction();
			}

			_looping = true;
			while(_looping)
			{
				float frameStartTime = getTime();

				// Handle events
				controllers::startFrame();
				SDL_Event sdlEvent;
				while(_looping && SDL_PollEvent(&sdlEvent))
				{
					handleSDLEvent(sdlEvent);
				}
				for(int i = 0; i < controllers::getNumControllers(); i++)
				{
					std::vector<std::pair<int, float>> controllerAxisEvents = controllers::getAxesChangedSinceLastFrame(i);
					for(auto controllerAxisEvent : controllerAxisEvents)
					{
						ControllerAxisEvent event{Ptr<Window>()};
						event.controller = i;
						event.axis = controllerAxisEvent.first;
						event.value = controllerAxisEvent.second;
						handleEvent(event);
					}
				}

				// Update
				for(auto window : _windows)
				{
					if(_looping)
					{
						UpdateEvent event = UpdateEvent(window);
						event.dt = 1.f / targetFrameRate;
						window->handleEvent(event);
					}
				}
				for(auto scene : _scenes)
				{
					UpdateEvent event = UpdateEvent(Ptr<Window>());
					event.dt = 1.f / targetFrameRate;
					scene->handleEvent(event);
				}

				// PreRender Update
				for(auto scene : _scenes)
				{
					scene->handleEvent(PreRenderUpdateEvent(Ptr<Window>()));
				}

				// Render
				for(auto window : _windows)
				{
					if(_looping)
					{
						window->render(_sdlGlContext);
					}
				}

				// FIX THIS: Introduce better loop timing
				float delayTime = (1.f / targetFrameRate) - (getTime() - frameStartTime);
				if(delayTime > 0)
				{
					SDL_Delay((unsigned int)(delayTime * 1000));
				}
			}
		}

		void showMessage(std::string const & message)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information", message.c_str(), nullptr);
		}

		void setCursorActive(bool)
		{
			// TODO
		}

		Ptr<Window> getWindowFromId(unsigned int id)
		{
			SDL_Window * sdlWindow = SDL_GetWindowFromID(id);
			if(sdlWindow != NULL)
			{
				for(auto window : _windows)
				{
					if(sdlWindow == window->getSDLWindow())
					{
						return window;
					}
				}
			}
			return Ptr<Window>();
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
			else if(SDLK_F1 <= sdlKey && sdlKey <= SDLK_F12)
			{
				return (KeyboardEvent::Key)((int)KeyboardEvent::F1 + (sdlKey - SDLK_F1));
			}
			else if(SDLK_F13 <= sdlKey && sdlKey <= SDLK_F15)
			{
				return (KeyboardEvent::Key)((int)KeyboardEvent::F13 + (sdlKey - SDLK_F13));
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

		void handleEvent(Event const& event)
		{
			if(event.window.isValid())
			{
				event.window->handleEvent(event);
			}
			else
			{
				for(auto window : _windows)
				{
					window->handleEvent(event);
				}
			}
			for(auto scene : _scenes)
			{
				scene->handleEvent(event);
			}
		}

		void handleSDLEvent(SDL_Event const & sdlEvent)
		{
			Ptr<Window> window;
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
							window->getCursor()->setWithinWindow(false);
							break;
						case SDL_WINDOWEVENT_ENTER:
							window->getCursor()->setWithinWindow(true);
							break;
					}
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					window = getWindowFromId(sdlEvent.key.windowID);
					if(window.isValid() && sdlEvent.key.repeat == 0) // only record actual press and releases
					{
						KeyboardEvent event(window);
						event.key = sdlKeyToKey(sdlEvent.key.keysym.sym);
						event.pressed = (sdlEvent.type == SDL_KEYDOWN ? true : false);
						handleEvent(event);
					}
					break;
				case SDL_TEXTINPUT:
					window = getWindowFromId(sdlEvent.text.windowID);
					if(window.isValid())
					{
						TextEvent event(window);
						event.text = sdlEvent.text.text;
						handleEvent(event);
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					window = getWindowFromId(sdlEvent.button.windowID);
					if(window.isValid())
					{
						MouseButtonEvent event(window);
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
					break;
				case SDL_MOUSEMOTION: // generates value changes for both moving and stopping
					window = getWindowFromId(sdlEvent.motion.windowID);
					if(window.isValid())
					{
						if(!_firstMouseMoveEvent) // the first mouse motion event always gives absolute values in its xrel and yrel, and so is invalid
						{
							MouseMoveEvent event(window);
							event.relative[0] = sdlEvent.motion.xrel;
							event.relative[1] = sdlEvent.motion.yrel;
							event.absolute[0] = sdlEvent.motion.x;
							event.absolute[1] = sdlEvent.motion.y;
							handleEvent(event);
						}
						else
						{
							_firstMouseMoveEvent = false;
						}
					}
					break;
				case SDL_MOUSEWHEEL:
					window = getWindowFromId(sdlEvent.wheel.windowID);
					if(window.isValid())
					{
						MouseWheelEvent event(window);
						event.up = (sdlEvent.wheel.y > 0);
						handleEvent(event);
					}
					break;
				case SDL_JOYBUTTONDOWN:
				case SDL_JOYBUTTONUP:
				{
					ControllerButtonEvent event(window);
					event.controller = sdlEvent.jbutton.which;
					event.button = sdlEvent.jbutton.button;
					event.pressed = (sdlEvent.type == SDL_JOYBUTTONDOWN);
					handleEvent(event);
					break;
				}
				case SDL_JOYAXISMOTION:
				{
					float value;
					if(sdlEvent.jaxis.value > 0)
					{
						value = (float)sdlEvent.jaxis.value / 32767.0f;
					}
					else
					{
						value = (float)sdlEvent.jaxis.value / 32768.0f;
					}
					controllers::updateControllerAxis(sdlEvent.jaxis.which, sdlEvent.jaxis.axis, value);
					break;
				}
				case SDL_JOYHATMOTION:
				{
					ControllerAxisEvent event(window);
					event.controller = sdlEvent.jhat.which;
					event.axis = controllers::getNumSDLAxes(sdlEvent.jhat.which) + sdlEvent.jhat.hat * 2 + 0;
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
					break;
				}
				case SDL_JOYBALLMOTION:
				{
					ControllerBallEvent event(window);
					event.controller = sdlEvent.jball.which;
					event.ball = sdlEvent.jball.ball;
					event.offset[0] = sdlEvent.jball.xrel;
					event.offset[1] = sdlEvent.jball.yrel;
					handleEvent(event);
					break;
				}
			}
		}
	}
}

