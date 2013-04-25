#include "App.h"
#include <SDL.h>
#include <SDL_opengl.h>
#ifdef __WIN32__
#include <windows.h>
#endif
#include <sstream>
#include <stdexcept>
#include <cassert>

namespace App
{
	bool mFullscreen = false;
	Vector2i mWindowSize(800, 600);
	bool mFirstMouseMoveEvent = true;
	std::vector<SDL_Joystick *> mJoysticks;
	std::vector<std::vector<Uint8> > mHatValues;
	bool mCursorVisible = true;
	bool mCursorEnabled = true;
	bool mCursorPositionValid = true;
	Vector2i mCursorPostion(0, 0);
	bool mRunning = false;
	float mLastTime = 0.0f;

	/*** Internal functions ***/

	int sdlKeyToKeyAxis(int sdlKey)
	{
		if(SDLK_a <= sdlKey && sdlKey <= SDLK_z)
		{
			return Key::A + (sdlKey - SDLK_a);
		}
		else if(SDLK_0 <= sdlKey && sdlKey <= SDLK_9)
		{
			return Key::N0 + (sdlKey - SDLK_0);
		}
		else if(SDLK_KP0 <= sdlKey && sdlKey <= SDLK_KP9)
		{
			return Key::Keypad0 + (sdlKey - SDLK_KP0);
		}
		else if(SDLK_F1 <= sdlKey && sdlKey <= SDLK_F15)
		{
			return Key::F1 + (sdlKey - SDLK_F1);
		}
		switch(sdlKey)
		{
			case SDLK_UP:
				return Key::Up;
			case SDLK_DOWN:
				return Key::Down;
			case SDLK_LEFT:
				return Key::Left;
			case SDLK_RIGHT:
				return Key::Right;
			case SDLK_PAGEUP:
				return Key::PageUp;
			case SDLK_PAGEDOWN:
				return Key::PageDown;
			case SDLK_HOME:
				return Key::Home;
			case SDLK_END:
				return Key::End;
			case SDLK_RETURN:
				return Key::Enter;
			case SDLK_BACKSPACE:
				return Key::Backspace;
			case SDLK_INSERT:
				return Key::Insert;
			case SDLK_DELETE:
				return Key::Delete;
			case SDLK_TAB:
				return Key::Tab;
			case SDLK_SPACE:
				return Key::Space;
			case SDLK_ESCAPE:
				return Key::Escape;
			case SDLK_KP_PLUS:
				return Key::KeypadAdd;
			case SDLK_KP_MINUS:
				return Key::KeypadSubtract;
			case SDLK_KP_MULTIPLY:
				return Key::KeypadMultiply;
			case SDLK_KP_DIVIDE:
				return Key::KeypadDivide;
			case SDLK_KP_PERIOD:
				return Key::KeypadDecimal;
			case SDLK_KP_ENTER:
				return Key::KeypadEnter;
			case SDLK_BACKQUOTE:
				return Key::Grave;
			case SDLK_QUOTE:
				return Key::Apostrophe;
			case SDLK_SEMICOLON:
				return Key::Semicolon;
			case SDLK_COMMA:
				return Key::Comma;
			case SDLK_PERIOD:
				return Key::Period;
			case SDLK_SLASH:
				return Key::Slash;
			case SDLK_LEFTBRACKET:
				return Key::LeftBracket;
			case SDLK_RIGHTBRACKET:
				return Key::RightBracket;
			case SDLK_BACKSLASH:
				return Key::Backslash;
			case SDLK_MINUS:
				return Key::Hyphen;
			case SDLK_EQUALS:
				return Key::Equals;
			case SDLK_CAPSLOCK:
				return Key::Capslock;
			case SDLK_LSHIFT:
				return Key::LeftShift;
			case SDLK_RSHIFT:
				return Key::RightShift;
			case SDLK_LCTRL:
				return Key::LeftControl;
			case SDLK_RCTRL:
				return Key::RightControl;
			case SDLK_LALT:
				return Key::LeftAlt;
			case SDLK_RALT:
				return Key::RightAlt;
			case SDLK_LMETA:
			case SDLK_LSUPER:
				return Key::LeftSystem;
			case SDLK_RMETA:
			case SDLK_RSUPER:
				return Key::RightSystem;
            case SDLK_PAUSE:
                return Key::Pause;
		}
		return -1;
	}

	void createEventsFromSDLEvent (std::vector<Event> & events, SDL_Event const& sdlEvent)
	{
		Event event;
		if(sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP)
		{
			event.device = Device::Keyboard;
			event.axis = sdlKeyToKeyAxis(sdlEvent.key.keysym.sym);
			if(event.axis == -1)
			{
				return;
			}
			event.value = (sdlEvent.type == SDL_KEYDOWN ? 1 : 0);
			events.push_back(event);
			if(sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.unicode != 0)
			{
				event.axis = Key::Char;
				event.value = sdlEvent.key.keysym.unicode;
				events.push_back(event);
			}
		}
		else if(sdlEvent.type == SDL_MOUSEMOTION) // generate value changes for both moving and stopping
		{
			if(!mFirstMouseMoveEvent) // the first mouse motion event always gives absolute values in its xrel and yrel, and so is invalid
			{
				event.device = Device::Keyboard;
				if(sdlEvent.motion.xrel != 0)
				{
					event.axis = Mouse::X;
					event.value = sdlEvent.motion.xrel;
					events.push_back(event);
					event.value = 0;
					events.push_back(event);
				}
				if(sdlEvent.motion.yrel != 0)
				{
					event.axis = Mouse::Y;
					event.value = sdlEvent.motion.yrel;
					events.push_back(event);
					event.value = 0;
					events.push_back(event);
				}
			}
			else
			{
				mFirstMouseMoveEvent = false;
			}
		}
		else if(sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP)
		{
			event.device = Device::Mouse;
			if(sdlEvent.button.button == SDL_BUTTON_WHEELDOWN || sdlEvent.button.button == SDL_BUTTON_WHEELUP)
			{
				if(sdlEvent.type == SDL_MOUSEBUTTONDOWN) // wheel move generates both buttonup and buttondown events, so only keep the buttondown ones
				{
					event.axis = Mouse::Wheel;
					event.value = (sdlEvent.button.button == SDL_BUTTON_WHEELDOWN ? -1 : +1);
					events.push_back(event);
				}
			}
			else
			{
				switch(sdlEvent.button.button)
				{
					case SDL_BUTTON_LEFT:
						event.axis = Mouse::Left; break;
					case SDL_BUTTON_MIDDLE:
						event.axis = Mouse::Middle; break;
					case SDL_BUTTON_RIGHT:
						event.axis = Mouse::Right; break;
					default:
						event.axis = Mouse::Button0 + sdlEvent.button.button;
				}
				event.value = (sdlEvent.type == SDL_MOUSEBUTTONDOWN ? 1 : 0);
				events.push_back(event);
			}
		}
		else if(sdlEvent.type == SDL_JOYAXISMOTION)
		{
			event.device = Device::Controller0 + sdlEvent.jaxis.which;
			event.axis = sdlEvent.jaxis.axis;
			event.value = sdlEvent.jaxis.value;
			events.push_back(event);
		}
		else if(sdlEvent.type == SDL_JOYBALLMOTION)
		{
			event.device = Device::Controller0 + sdlEvent.jaxis.which;
			if(sdlEvent.jball.xrel != 0)
			{
				event.axis = 256 + 2 * sdlEvent.jball.ball;
				event.value = sdlEvent.jball.xrel;
				events.push_back(event);
				event.value = 0;
				events.push_back(event);
			}
			if(sdlEvent.jball.yrel != 0)
			{
				event.axis = 256 + 2 * sdlEvent.jball.ball + 1;
				event.value = sdlEvent.jball.yrel;
				events.push_back(event);
				event.value = 0;
				events.push_back(event);
			}
		}
		else if(sdlEvent.type == SDL_JOYHATMOTION)
		{
			event.device = Device::Controller0 + sdlEvent.jaxis.which;
			Uint8 prevHatValue = mHatValues[sdlEvent.jaxis.which][sdlEvent.jhat.hat];
			if((sdlEvent.jhat.value & SDL_HAT_LEFT) != (prevHatValue & SDL_HAT_LEFT) || ((sdlEvent.jhat.value & SDL_HAT_RIGHT) != (prevHatValue & SDL_HAT_RIGHT)))
			{
				event.axis = 512 + 2 * sdlEvent.jhat.hat;
				if((sdlEvent.jhat.value & SDL_HAT_RIGHT) != 0)
				{
					event.value = +1;
				}
				else if((sdlEvent.jhat.value & SDL_HAT_LEFT) != 0)
				{
					event.value = -1;
				}
				else
				{
					event.value = 0;
				}
				events.push_back(event);
			}
			if((sdlEvent.jhat.value & SDL_HAT_DOWN) != (prevHatValue & SDL_HAT_DOWN) || ((sdlEvent.jhat.value & SDL_HAT_UP) != (prevHatValue & SDL_HAT_UP)))
			{
				event.axis = 512 + 2 * sdlEvent.jhat.hat + 1;
				if((sdlEvent.jhat.value & SDL_HAT_UP) != 0)
				{
					event.value = +1;
				}
				else if((sdlEvent.jhat.value & SDL_HAT_DOWN) != 0)
				{
					event.value = -1;
				}
				else
				{
					event.value = 0;
				}
				events.push_back(event);
			}
			mHatValues[sdlEvent.jaxis.which][sdlEvent.jhat.hat] = sdlEvent.jhat.value;
		}
		else if(sdlEvent.type == SDL_JOYBUTTONDOWN || sdlEvent.type == SDL_JOYBUTTONUP)
		{
			event.device = Device::Controller0 + sdlEvent.jaxis.which;
			event.axis = 768 + sdlEvent.jbutton.button;
			event.value = (sdlEvent.type == SDL_JOYBUTTONDOWN ? 1 : 0);
			events.push_back(event);
		}
	}

	void handleEvents()
	{
		SDL_Event event;
		std::vector<Event> events;
		SDL_PumpEvents();
		while(SDL_PollEvent(&event) == 1)
		{
			if(event.type == SDL_QUIT)
			{
				quit();
			}
			else if(event.type == SDL_VIDEOEXPOSE || event.type == SDL_USEREVENT || event.type == SDL_SYSWMEVENT)
			{ // Do nothing.
			}
			else if(event.type == SDL_VIDEORESIZE)
			{
				if(!mFullscreen)
				{
					Coord size(event.resize.w, event.resize.h);
					setSize(mFullscreen, size);
					onResize(mWindowSize); // calls user-defined function
					mLastTime = SDL_GetTicks() / 1000.0f;
				}
			}
			else if(event.type == SDL_ACTIVEEVENT)
			{
				if(mFullscreen && (event.active.type & SDL_APPACTIVE) != 0 && event.active.gain == 0)
				{
					setSize(true, getSize());
				}
				if((event.active.type & SDL_APPMOUSEFOCUS) != 0)
				{
					mCursorPositionValid = (event.active.gain == 1);
				}
			}
			else
			{
				createEventsFromSDLEvent(events, event);
			}
		}
		if(cursorHasValidPosition())
		{
			Coord position = cursorPosition();
			event.type = SDL_MOUSEMOTION;
			event.motion.x = (Uint16)cursor_pos.x;
			event.motion.y = (Uint16)cursor_pos.y;
			event.motion.xrel = 0;
			event.motion.yrel = 0;
			event.motion.state = 0;
			createEventsFromSDLEvent(events, event);
		}
		for(unsigned int i = 0; i < events.size(); ++i)
		{
			if(events[i].device == Device::Mouse && (events[i].axis == Mouse::X || events[i].axis == Mouse::Y))
			{
				mCursorPositionValid = true;
				if(mCursorEnabled)
				{
					SDL_GetMouseState(&mCursorPosition.x, &mCursorPosition.y);
				}
			}
			onEvent(events[i]); // calls user-defined function
		}
	}

	void draw()
	{
		// mostly reset to basic state
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		for(int i = 0; i < GL_MAX_LIGHTS; i++)
		{
			glDisable(GL_LIGHT0 + i);
		}
		glColor4f(1, 1, 1, 1);

		// clear window
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 0, m_window_size.x, m_window_size.y);
		glViewport(0, 0, m_window_size.x, m_window_size.y);
		glClearColor(0, 0, 0, 0);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// reset the matrices
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		onRender(); // calls user-defined function

		SDL_GL_SwapBuffers();
	}

	/*** Header functions ***/

	void showMessage(std::string const& text)
	{
	#ifdef __WIN32__
		MessageBoxA(NULL, text.c_str(), "Message", MB_OK);
	#else
		printf("%s", text.c_str());
	#endif
	}

	void setTitle(std::string const& title)
	{
		SDL_WM_SetCaption(title.c_str(), title.c_str());
	}

	Coord getSize()
	{
		return mWindowSize;
	}

	bool isFullscreen()
	{
		return mFullscreen;
	}

	void setSize(bool fullscreen, Coord windowSize)
	{
		Uint32 flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;
		if(fullscreen)
		{
			flags |= SDL_FULLSCREEN;
		}
		else
		{
			flags |= SDL_ANYFORMAT | SDL_RESIZABLE;
		}
		if(SDL_SetVideoMode(windowSize.x, windowSize.y, 32, flags) == NULL)
		{
			std::stringstream ss;
			ss << "A window size of " << windowSize.x << "x" << windowSize.y << " is not supported while in " << (fullscreen ? "fullscreen" : "windowed") << " mode. ";
			throw std::runtime_error(ss.str());
		}
		mWindowSize = windowSize;
		mFullscreen = fullscreen;
	}

	void startLoop()
	{
		mRunning = true;
		mLastTime = SDL_GetTicks() / 1000.0f;
		while(mRunning)
		{
			float newTime = SDL_GetTicks() / 1000.0f;
			float deltaTime = newTime - mLastTime; // calculate the last frame's duration
			mLastTime = newTime;
			if(mRunning)
			{
				handleEvents();
			}
			if(mRunning)
			{
				onFrame(deltaTime); // calls user-defined function
			}
			if(mRunning)
			{
				draw();
			}
		}
	}

	void quit()
	{
		mRunning = false;
	}

	std::vector<Coord> getAllResolutions()
	{
		std::vector<Size > resolutions;
		SDL_Rect ** rects = SDL_ListModes(0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN);
		for(int i = 0; rects[i] != 0; ++i)
			resolutions.push_back(Size(rects[i]->w, rects[i]->h));
		return resolutions;
	}

	int getNumInputDevices()
	{
		return 2 + (int)m_joysticks.size();
	}

	void refreshInputDevices()
	{
		if(SDL_WasInit(SDL_INIT_EVERYTHING) & SDL_INIT_JOYSTICK)
		{
			for(int i = 0; i < (int)mJoysticks.size(); ++i)
			{
				SDL_JoystickClose(mJoysticks[i]);
			}
			SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
		}
		if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) == 0)
		{
			int numJoysticks = SDL_NumJoysticks();
			for(int i = 0; i < numJoysticks; ++i)
			{
				SDL_Joystick * joystick = SDL_JoystickOpen(i);
				if(joystick == NULL)
				{
					std::stringstream ss;
					ss << "Could not initialize input controller " << (i + 1) << ". ";
					throw std::runtime_error(ss.str());
				}
				std::vector<Uint8> hatValue;
				int numHats = SDL_JoystickNumHats(joystick);
				for(int i = 0; i < numHats; ++i)
				{
					hatValue.push_back(SDL_HAT_CENTERED);
				]
				mJoysticks.push_back(joystick);
				mHatValues.push_back(hatValue);
			}
		}
	}

	bool hasValidCursorPosition()
	{
		return mCursorEnabled && mCursorPositionValid;
	}

	Coord getCursorPosition()
	{
		assert(mCursorEnabled && mCursorPositionValid);
		return mCursorPosition;
	}

	bool isCursorVisible()
	{
		return mCursorVisible;
	}

	void setCursorVisible (bool visible)
	{
		mCursorVisible = visible;
		if(mCursorEnabled)
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
	}

	bool isCursorEnabled()
	{
		return mCursorEnabled;
	}

	void setCursorEnabled(bool enabled)
	{
		mCursorEnabled = enabled;
		if(enabled)
		{
			if(mCursorVisible)
			{
				SDL_ShowCursor(SDL_ENABLE);
			}
			SDL_WM_GrabInput(SDL_GRAB_OFF);
			SDL_WarpMouse((Uint16)mCursorPosition.x, (Uint16)mCursorPosition.y);
		}
		else
		{
			SDL_ShowCursor(SDL_DISABLE);
			SDL_WM_GrabInput(SDL_GRAB_ON);
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
    
    // Open the window.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
    }
    SDL_EnableUNICODE(SDL_ENABLE);
    setWindow(false, Coord(800, 600));
    refreshInputDevices();
    
    // Run the user onEntry function.
	try
	{
		app::onEntry(params); // calls user-defined function
	}
	catch(std::runtime_error const& err)
	{
		app::showMessage(err.what());
	}
	return 0;
    
    // Close window.
    for(int i = 0; i < (int)mJoysticks.size(); ++i)
    {
        SDL_JoystickClose(mJoysticks[i]);
    }
    SDL_Quit();
}
