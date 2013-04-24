#include `"Application.h"
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
	Size mWindowSize(800, 600);
	bool mFirstMouseMoveEvent = true;
	std::vector<SDL_Joystick *> mJoysticks;
	std::vector<std::vector<Uint8> > mHatValues;
	bool mCursorVisible = true;
	bool mCursorEnabled = true;
	bool mCursorPositionValid = true;
	Size mCursorPostion(0, 0);
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
			return Key::N_0 + (sdlKey - SDLK_0);
		}
		else if(SDLK_KP0 <= sdlKey && sdlKey <= SDLK_KP9)
		{
			return Key::KP_0 + (sdlKey - SDLK_KP0);
		}
		else if(SDLK_F1 <= sdlKey && sdlKey <= SDLK_F15)
		{
			return Key::F_1 + (sdlKey - SDLK_F1);
		}
		switch(sdlKey)
		{
			case SDLK_UP:
				return Key::UP;
			case SDLK_DOWN:
				return Key::DOWN;
			case SDLK_LEFT:
				return Key::LEFT;
			case SDLK_RIGHT:
				return Key::RIGHT;
			case SDLK_PAGEUP:
				return Key::PAGE_UP;
			case SDLK_PAGEDOWN:
				return Key::PAGE_DOWN;
			case SDLK_HOME:
				return Key::HOME;
			case SDLK_END:
				return Key::END;
			case SDLK_RETURN:
				return Key::ENTER;
			case SDLK_BACKSPACE:
				return Key::BACKSPACE;
			case SDLK_INSERT:
				return Key::INSERT;
			case SDLK_DELETE:
				return Key::DELETE;
			case SDLK_TAB:
				return Key::TAB;
			case SDLK_SPACE:
				return Key::SPACE;
			case SDLK_ESCAPE:
				return Key::ESCAPE;
			case SDLK_KP_PLUS:
				return Key::KP_ADD;
			case SDLK_KP_MINUS:
				return Key::KP_SUBTRACT;
			case SDLK_KP_MULTIPLY:
				return Key::KP_MULTIPLY;
			case SDLK_KP_DIVIDE:
				return Key::KP_DIVIDE;
			case SDLK_KP_PERIOD:
				return Key::KP_DECIMAL;
			case SDLK_KP_ENTER:
				return Key::KP_ENTER;
			case SDLK_BACKQUOTE:
				return Key::GRAVE;
			case SDLK_QUOTE:
				return Key::APOSTROPHE;
			case SDLK_SEMICOLON:
				return Key::SEMICOLON;
			case SDLK_COMMA:
				return Key::COMMA;
			case SDLK_PERIOD:
				return Key::PERIOD;
			case SDLK_SLASH:
				return Key::SLASH;
			case SDLK_LEFTBRACKET:
				return Key::L_BRACKET;
			case SDLK_RIGHTBRACKET:
				return Key::R_BRACKET;
			case SDLK_BACKSLASH:
				return Key::BACKSLASH;
			case SDLK_MINUS:
				return Key::HYPHEN;
			case SDLK_EQUALS:
				return Key::EQUALS;
			case SDLK_CAPSLOCK:
				return Key::CAPSLOCK;
			case SDLK_LSHIFT:
				return Key::L_SHIFT;
			case SDLK_RSHIFT:
				return Key::R_SHIFT;
			case SDLK_LCTRL:
				return Key::L_CTRL;
			case SDLK_RCTRL:
				return Key::R_CTRL;
			case SDLK_LALT:
				return Key::L_ALT;
			case SDLK_RALT:
				return Key::R_ALT;
			case SDLK_LMETA:
			case SDLK_LSUPER:
				return Key::L_SYSTEM;
			case SDLK_RMETA:
			case SDLK_RSUPER:
				return Key::R_SYSTEM;
		}
		return -1;
	}

	void createEventsFromSDLEvent (std::vector<Event> & events, SDL_Event const& sdlEvent)
	{
		Event event;
		if(sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP)
		{
			event.device = Device::KEYBOARD;
			event.axis = sdlKeyToKeyAxis(sdlEvent.key.keysym.sym);
			if(event.axis == -1)
			{
				return;
			}
			event.value = (sdlEvent.type == SDL_KEYDOWN ? 1 : 0);
			events.push_back(event);
			if(sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.unicode != 0)
			{
				event.axis = Key::CHAR;
				event.value = sdlEvent.key.keysym.unicode;
				events.push_back(event);
			}
		}
		else if(sdlEvent.type == SDL_MOUSEMOTION) // generate value changes for both moving and stopping
		{
			if(!mFirstMouseMoveEvent) // the first mouse motion event always gives absolute values in its xrel and yrel, and so is invalid
			{
				event.device = Device::KEYBOARD;
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
			event.device = Device::MOUSE;
			if((sdlEvent.button.button == SDL_BUTTON_WHEELDOWN || sdlEvent.button.button == SDL_BUTTON_WHEELUP)) 
			{
				if(sdlEvent.type == SDL_MOUSEBUTTONDOWN) // wheel move generates both buttonup and buttondown events, so only keep the buttondown ones
				{
					event.axis = Mouse::WHEEL;
					event.value = (sdlEvent.button.button == SDL_BUTTON_WHEELDOWN ? -1 : +1);
					events.push_back(event);
				}
			}
			else 
			{
				switch(sdlEvent.button.button)
				{
					case SDL_BUTTON_LEFT:
						event.axis = Mouse::LEFT; break;
					case SDL_BUTTON_MIDDLE:
						event.axis = Mouse::MIDDLE; break;
					case SDL_BUTTON_RIGHT:
						event.axis = Mouse::RIGHT; break;
					default:
						event.axis = Mouse::BUTTON_0 + sdlEvent.button.button;
				}
				event.value = (sdlEvent.type == SDL_MOUSEBUTTONDOWN ? 1 : 0);
				events.push_back(event);
			}
		}
		else if(sdlEvent.type == SDL_JOYAXISMOTION) 
		{
			event.device = Device::CONTROLLER_0 + sdlEvent.jaxis.which;
			event.axis = sdlEvent.jaxis.axis;
			event.value = sdlEvent.jaxis.value;
			events.push_back(event);
		}
		else if(sdlEvent.type == SDL_JOYBALLMOTION) 
		{
			event.device = Device::CONTROLLER_0 + sdlEvent.jaxis.which;
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
			event.device = Device::CONTROLLER_0 + sdlEvent.jaxis.which;
			Uint8 prevHatValue = mHatValues[sdlEvent.jaxis.which][sdlEvent.jhat.hat];
			if(((sdlEvent.jhat.value & SDL_HAT_LEFT) != (prevHatValue & SDL_HAT_LEFT)) || (((sdlEvent.jhat.value & SDL_HAT_RIGHT) != (prevHatValue & SDL_HAT_RIGHT)))) 
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
			if(((sdlEvent.jhat.value & SDL_HAT_DOWN) != (prevHatValue & SDL_HAT_DOWN)) || (((sdlEvent.jhat.value & SDL_HAT_UP) != (prevHatValue & SDL_HAT_UP)))) 
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
			event.device = Device::CONTROLLER_0 + sdlEvent.jaxis.which;
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
			if(events[i].device == Device::MOUSE && (events[i].axis == Mouse::X || events[i].axis == Mouse::Y)) 
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

	void openWindow() 
	{
		if(SDL_Init(SDL_INIT_VIDEO) == -1)
		{
			throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
		}
		SDL_EnableUNICODE(SDL_ENABLE);
		setWindow(false, Coord(800, 600));
		refreshInputDevices();
	}

	void closeWindow() 
	{
		for(int i = 0; i < (int)mJoysticks.size(); ++i)
		{
			SDL_JoystickClose(mJoysticks[i]);
		}
		SDL_Quit();
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
				on_frame(deltaTime); // calls user-defined function
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

	std::string getDeviceName(int device) 
	{
		if(device == Device::KEYBOARD)
		{
			return "keyboard";
		}
		else if(device == Device::MOUSE)
		{
			return "mouse";
		}
		else if(device >= Device::CONTROLLER_0) 
		{
			std::stringstream ss;
			ss << "controller " << (device - Device::CONTROLLER_0);
			return ss.str();
		}
		else
		{
			return "unknown device";
		}
	}

	std::string getAxisName(int device, int axis) 
	{
		if(device == Device::KEYBOARD) 
		{
			switch(axis)
			{
				case Key::A:
					return "a";
				case Key::B:
					return "b";
				case Key::C:
					return "c";
				case Key::D:
					return "d";
				case Key::E:
					return "e";
				case Key::F:
					return "f";
				case Key::G:
					return "g";
				case Key::H:
					return "h";
				case Key::I:
					return "i";
				case Key::J:
					return "j";
				case Key::K:
					return "k";
				case Key::L:
					return "l";
				case Key::M:
					return "m";
				case Key::N:
					return "n";
				case Key::O:
					return "o";
				case Key::P:
					return "p";
				case Key::Q:
					return "q";
				case Key::R:
					return "r";
				case Key::S:
					return "s";
				case Key::T:
					return "t";
				case Key::U:
					return "u";
				case Key::V:
					return "v";
				case Key::W:
					return "w";
				case Key::X:
					return "x";
				case Key::Y:
					return "y";
				case Key::Z:
					return "z";
				case Key::N_0:
					return "0";
				case Key::N_1:
					return "1";
				case Key::N_2:
					return "2";
				case Key::N_3:
					return "3";
				case Key::N_4:
					return "4";
				case Key::N_5:
					return "5";
				case Key::N_6:
					return "6";
				case Key::N_7:
					return "7";
				case Key::N_8:
					return "8";
				case Key::N_9:
					return "9";
				case Key::UP:
					return "up";
				case Key::DOWN:
					return "down";
				case Key::LEFT:
					return "left";
				case Key::RIGHT:
					return "right";
				case Key::PAGE_UP:
					return "page up";
				case Key::PAGE_DOWN:
					return "page down";
				case Key::HOME:
					return "home";
				case Key::END:
					return "end";
				case Key::ENTER:
					return "enter";
				case Key::BACKSPACE:
					return "backspace";
				case Key::INSERT:
					return "insert";
				case Key::DELETE:
					return "delete";
				case Key::TAB:
					return "tab";
				case Key::SPACE:
					return "space";
				case Key::ESCAPE:
					return "escape";
				case Key::KP_0:
					return "keypad 0";
				case Key::KP_1:
					return "keypad 1";
				case Key::KP_2:
					return "keypad 2";
				case Key::KP_3:
					return "keypad 3";
				case Key::KP_4:
					return "keypad 4";
				case Key::KP_5:
					return "keypad 5";
				case Key::KP_6:
					return "keypad 6";
				case Key::KP_7:
					return "keypad 7";
				case Key::KP_8:
					return "keypad 8";
				case Key::KP_9:
					return "keypad 9";
				case Key::KP_ADD:
					return "keypad plus";
				case Key::KP_SUBTRACT:
					return "keypad minus";
				case Key::KP_MULTIPLY:
					return "keypad multiply";
				case Key::KP_DIVIDE:
					return "keypad divide";
				case Key::KP_DECIMAL:
					return "keypad decimal";
				case Key::KP_ENTER:
					return "kp enter";
				case Key::GRAVE:
					return "grave";
				case Key::APOSTROPHE:
					return "apostrophe";
				case Key::SEMICOLON:
					return "semicolon";
				case Key::COMMA:
					return "comma";
				case Key::PERIOD:
					return "period";
				case Key::SLASH:
					return "slash";
				case Key::L_BRACKET:
					return "left bracket";
				case Key::R_BRACKET:
					return "right bracket";
				case Key::BACKSLASH:
					return "backslash";
				case Key::HYPHEN:
					return "hyphen";
				case Key::EQUALS:
					return "equals";
				case Key::CAPSLOCK:
					return "capslock";
				case Key::L_SHIFT:
					return "left shift";
				case Key::R_SHIFT:
					return "right shift";
				case Key::L_CTRL:
					return "left ctrl";
				case Key::R_CTRL:
					return "right ctrl";
				case Key::L_ALT:
					return "left alt";
				case Key::R_ALT:
					return "right alt";
				case Key::L_SYSTEM:
					return "left system";
				case Key::R_SYSTEM:
					return "right system";
				case Key::F_1:
					return "f1";
				case Key::F_2:
					return "f2";
				case Key::F_3:
					return "f3";
				case Key::F_4:
					return "f4";
				case Key::F_5:
					return "f5";
				case Key::F_6:
					return "f6";
				case Key::F_7:
					return "f7";
				case Key::F_8:
					return "f8";
				case Key::F_9:
					return "f9";
				case Key::F_10:
					return "f10";
				case Key::F_11:
					return "f11";
				case Key::F_12:
					return "f12";
				case Key::F_13:
					return "f13";
				case Key::F_14:
					return "f14";
				case Key::F_15:
					return "f15";
				case Key::CHAR:
					return "unicode";
			}
		}
		else if(device == Device::MOUSE) 
		{
			switch(axis)
			{
				case Mouse::X:
					return "x axis";
				case Mouse::Y:
					return "y axis";
				case Mouse::WHEEL:
					return "wheel";
				case Mouse::LEFT:
					return "left button";
				case Mouse::MIDDLE:
					return "middle button";
				case Mouse::RIGHT:
					return "right button";
				default:
					if(axis >= Mouse::BUTTON_0)
					{
						std::stringstream ss;
						ss << "button " << (axis - Mouse::BUTTON_0);
						return ss.str();
					}
		}
		else if(deivce >= Device::CONTROLLER_0) 
		{
			if(0 <= axis && axis < 256) 
			{
				std::stringstream ss;
				ss << "axis " << (axis + 1);
				return ss.str();
			}
			else if(256 <= axis && axis < 512) 
			{
				std::stringstream ss;
				ss << "ball " << ((axis - 256)/2 + 1) << " " << ((axis - 256) % 2 == 0 ? "x" : "y");
				return ss.str();
			}
			else if(512 <= axis && axis < 768) 
			{
				std::stringstream ss;
				ss << "hat " << ((axis - 512)/2 + 1) << " " << ((axis - 512) % 2 == 0 ? "x" : "y");
				return ss.str();
			}
			else if(768 <= axis && axis < 1024) 
			{
				std::stringstream ss;
				ss << "button " << (axis - 768 + 1);
				return ss.str();
			}
		}
		return "unknown axis";
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
	std::vector<std::string> params;
	for(int i = 1; i < argc; ++i)
	{
		params.push_back(std::string(argv[i]));
	}
	try 
	{
		app::onEntry(params); // calls user-defined function
	}
	catch(std::runtime_error const& err) 
	{
		app::showMessage(err.what());
	}
	return 0;
}

