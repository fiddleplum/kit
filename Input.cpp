#include "Input.h"
#include <string>
#include <map>
#include <stdexcept>
#include <vector>
#include <SDL.h>

std::map<int, std::string> gKeyToNameMap;
bool gFirstMouseMoveEvent = true;
std::vector<SDL_Joystick *> gJoysticks;
std::vector<std::vector<Uint8> > gHatValues;
bool gCursorVisible = true;
bool gCursorEnabled = true;
bool gCursorPositionValid = true;
Vector2i gCursorPosition(0, 0);

void setupKeyToNameMap()
{
	gKeyToNameMap[Input::Keyboard::A] = "a";
	gKeyToNameMap[Input::Keyboard::B] = "b";
	gKeyToNameMap[Input::Keyboard::C] = "c";
	gKeyToNameMap[Input::Keyboard::D] = "d";
	gKeyToNameMap[Input::Keyboard::E] = "e";
	gKeyToNameMap[Input::Keyboard::F] = "f";
	gKeyToNameMap[Input::Keyboard::G] = "g";
	gKeyToNameMap[Input::Keyboard::H] = "h";
	gKeyToNameMap[Input::Keyboard::I] = "i";
	gKeyToNameMap[Input::Keyboard::J] = "j";
	gKeyToNameMap[Input::Keyboard::K] = "k";
	gKeyToNameMap[Input::Keyboard::L] = "l";
	gKeyToNameMap[Input::Keyboard::M] = "m";
	gKeyToNameMap[Input::Keyboard::N] = "n";
	gKeyToNameMap[Input::Keyboard::O] = "o";
	gKeyToNameMap[Input::Keyboard::P] = "p";
	gKeyToNameMap[Input::Keyboard::Q] = "q";
	gKeyToNameMap[Input::Keyboard::R] = "r";
	gKeyToNameMap[Input::Keyboard::S] = "s";
	gKeyToNameMap[Input::Keyboard::T] = "t";
	gKeyToNameMap[Input::Keyboard::U] = "u";
	gKeyToNameMap[Input::Keyboard::V] = "v";
	gKeyToNameMap[Input::Keyboard::W] = "w";
	gKeyToNameMap[Input::Keyboard::X] = "x";
	gKeyToNameMap[Input::Keyboard::Y] = "y";
	gKeyToNameMap[Input::Keyboard::Z] = "z";
	gKeyToNameMap[Input::Keyboard::N0] = "0";
	gKeyToNameMap[Input::Keyboard::N1] = "1";
	gKeyToNameMap[Input::Keyboard::N2] = "2";
	gKeyToNameMap[Input::Keyboard::N3] = "3";
	gKeyToNameMap[Input::Keyboard::N4] = "4";
	gKeyToNameMap[Input::Keyboard::N5] = "5";
	gKeyToNameMap[Input::Keyboard::N6] = "6";
	gKeyToNameMap[Input::Keyboard::N7] = "7";
	gKeyToNameMap[Input::Keyboard::N8] = "8";
	gKeyToNameMap[Input::Keyboard::N9] = "9";
	gKeyToNameMap[Input::Keyboard::Up] = "up";
	gKeyToNameMap[Input::Keyboard::Down] = "down";
	gKeyToNameMap[Input::Keyboard::Left] = "left";
	gKeyToNameMap[Input::Keyboard::Right] = "right";
	gKeyToNameMap[Input::Keyboard::PageUp] = "page up";
	gKeyToNameMap[Input::Keyboard::PageDown] = "page down";
	gKeyToNameMap[Input::Keyboard::Home] = "home";
	gKeyToNameMap[Input::Keyboard::End] = "end";
	gKeyToNameMap[Input::Keyboard::Enter] = "enter";
	gKeyToNameMap[Input::Keyboard::Backspace] = "backspace";
	gKeyToNameMap[Input::Keyboard::Insert] = "insert";
	gKeyToNameMap[Input::Keyboard::Delete] = "delete";
	gKeyToNameMap[Input::Keyboard::Tab] = "tab";
	gKeyToNameMap[Input::Keyboard::Space] = "space";
	gKeyToNameMap[Input::Keyboard::Escape] = "escape";
	gKeyToNameMap[Input::Keyboard::Keypad0] = "keypad 0";
	gKeyToNameMap[Input::Keyboard::Keypad1] = "keypad 1";
	gKeyToNameMap[Input::Keyboard::Keypad2] = "keypad 2";
	gKeyToNameMap[Input::Keyboard::Keypad3] = "keypad 3";
	gKeyToNameMap[Input::Keyboard::Keypad4] = "keypad 4";
	gKeyToNameMap[Input::Keyboard::Keypad5] = "keypad 5";
	gKeyToNameMap[Input::Keyboard::Keypad6] = "keypad 6";
	gKeyToNameMap[Input::Keyboard::Keypad7] = "keypad 7";
	gKeyToNameMap[Input::Keyboard::Keypad8] = "keypad 8";
	gKeyToNameMap[Input::Keyboard::Keypad9] = "keypad 9";
	gKeyToNameMap[Input::Keyboard::KeypadAdd] = "keypad plus";
	gKeyToNameMap[Input::Keyboard::KeypadSubtract] = "keypad minus";
	gKeyToNameMap[Input::Keyboard::KeypadMultiply] = "keypad multiply";
	gKeyToNameMap[Input::Keyboard::KeypadDivide] = "keypad divide";
	gKeyToNameMap[Input::Keyboard::KeypadDecimal] = "keypad decimal";
	gKeyToNameMap[Input::Keyboard::KeypadEnter] = "keypad enter";
	gKeyToNameMap[Input::Keyboard::Grave] = "grave";
	gKeyToNameMap[Input::Keyboard::Apostrophe] = "apostrophe";
	gKeyToNameMap[Input::Keyboard::Semicolon] = "semicolon";
	gKeyToNameMap[Input::Keyboard::Comma] = "comma";
	gKeyToNameMap[Input::Keyboard::Period] = "period";
	gKeyToNameMap[Input::Keyboard::Slash] = "slash";
	gKeyToNameMap[Input::Keyboard::LeftBracket] = "left bracket";
	gKeyToNameMap[Input::Keyboard::RightBracket] = "right bracket";
	gKeyToNameMap[Input::Keyboard::Backslash] = "backslash";
	gKeyToNameMap[Input::Keyboard::Hyphen] = "hyphen";
	gKeyToNameMap[Input::Keyboard::Equals] = "equals";
	gKeyToNameMap[Input::Keyboard::Capslock] = "capslock";
	gKeyToNameMap[Input::Keyboard::LeftShift] = "left shift";
	gKeyToNameMap[Input::Keyboard::RightShift] = "right shift";
	gKeyToNameMap[Input::Keyboard::LeftControl] = "left control";
	gKeyToNameMap[Input::Keyboard::RightControl] = "right control";
	gKeyToNameMap[Input::Keyboard::LeftAlt] = "left alt";
	gKeyToNameMap[Input::Keyboard::RightAlt] = "right alt";
	gKeyToNameMap[Input::Keyboard::LeftSystem] = "left system";
	gKeyToNameMap[Input::Keyboard::RightSystem] = "right system";
	gKeyToNameMap[Input::Keyboard::F1] = "f1";
	gKeyToNameMap[Input::Keyboard::F2] = "f2";
	gKeyToNameMap[Input::Keyboard::F3] = "f3";
	gKeyToNameMap[Input::Keyboard::F4] = "f4";
	gKeyToNameMap[Input::Keyboard::F5] = "f5";
	gKeyToNameMap[Input::Keyboard::F6] = "f6";
	gKeyToNameMap[Input::Keyboard::F7] = "f7";
	gKeyToNameMap[Input::Keyboard::F8] = "f8";
	gKeyToNameMap[Input::Keyboard::F9] = "f9";
	gKeyToNameMap[Input::Keyboard::F10] = "f10";
	gKeyToNameMap[Input::Keyboard::F11] = "f11";
	gKeyToNameMap[Input::Keyboard::F12] = "f12";
	gKeyToNameMap[Input::Keyboard::F13] = "f13";
	gKeyToNameMap[Input::Keyboard::F14] = "f14";
	gKeyToNameMap[Input::Keyboard::F14] = "f15";
	gKeyToNameMap[Input::Keyboard::Pause] = "pause";
	gKeyToNameMap[Input::Keyboard::Char] = "unicode";
}

int sdlKeyToKeyAxis(int sdlKey)
{
	if(SDLK_a <= sdlKey && sdlKey <= SDLK_z)
	{
		return Input::Keyboard::A + (sdlKey - SDLK_a);
	}
	else if(SDLK_0 <= sdlKey && sdlKey <= SDLK_9)
	{
		return Input::Keyboard::N0 + (sdlKey - SDLK_0);
	}
	else if(SDLK_KP0 <= sdlKey && sdlKey <= SDLK_KP9)
	{
		return Input::Keyboard::Keypad0 + (sdlKey - SDLK_KP0);
	}
	else if(SDLK_F1 <= sdlKey && sdlKey <= SDLK_F15)
	{
		return Input::Keyboard::F1 + (sdlKey - SDLK_F1);
	}
	switch(sdlKey)
	{
		case SDLK_UP:
			return Input::Keyboard::Up;
		case SDLK_DOWN:
			return Input::Keyboard::Down;
		case SDLK_LEFT:
			return Input::Keyboard::Left;
		case SDLK_RIGHT:
			return Input::Keyboard::Right;
		case SDLK_PAGEUP:
			return Input::Keyboard::PageUp;
		case SDLK_PAGEDOWN:
			return Input::Keyboard::PageDown;
		case SDLK_HOME:
			return Input::Keyboard::Home;
		case SDLK_END:
			return Input::Keyboard::End;
		case SDLK_RETURN:
			return Input::Keyboard::Enter;
		case SDLK_BACKSPACE:
			return Input::Keyboard::Backspace;
		case SDLK_INSERT:
			return Input::Keyboard::Insert;
		case SDLK_DELETE:
			return Input::Keyboard::Delete;
		case SDLK_TAB:
			return Input::Keyboard::Tab;
		case SDLK_SPACE:
			return Input::Keyboard::Space;
		case SDLK_ESCAPE:
			return Input::Keyboard::Escape;
		case SDLK_KP_PLUS:
			return Input::Keyboard::KeypadAdd;
		case SDLK_KP_MINUS:
			return Input::Keyboard::KeypadSubtract;
		case SDLK_KP_MULTIPLY:
			return Input::Keyboard::KeypadMultiply;
		case SDLK_KP_DIVIDE:
			return Input::Keyboard::KeypadDivide;
		case SDLK_KP_PERIOD:
			return Input::Keyboard::KeypadDecimal;
		case SDLK_KP_ENTER:
			return Input::Keyboard::KeypadEnter;
		case SDLK_BACKQUOTE:
			return Input::Keyboard::Grave;
		case SDLK_QUOTE:
			return Input::Keyboard::Apostrophe;
		case SDLK_SEMICOLON:
			return Input::Keyboard::Semicolon;
		case SDLK_COMMA:
			return Input::Keyboard::Comma;
		case SDLK_PERIOD:
			return Input::Keyboard::Period;
		case SDLK_SLASH:
			return Input::Keyboard::Slash;
		case SDLK_LEFTBRACKET:
			return Input::Keyboard::LeftBracket;
		case SDLK_RIGHTBRACKET:
			return Input::Keyboard::RightBracket;
		case SDLK_BACKSLASH:
			return Input::Keyboard::Backslash;
		case SDLK_MINUS:
			return Input::Keyboard::Hyphen;
		case SDLK_EQUALS:
			return Input::Keyboard::Equals;
		case SDLK_CAPSLOCK:
			return Input::Keyboard::Capslock;
		case SDLK_LSHIFT:
			return Input::Keyboard::LeftShift;
		case SDLK_RSHIFT:
			return Input::Keyboard::RightShift;
		case SDLK_LCTRL:
			return Input::Keyboard::LeftControl;
		case SDLK_RCTRL:
			return Input::Keyboard::RightControl;
		case SDLK_LALT:
			return Input::Keyboard::LeftAlt;
		case SDLK_RALT:
			return Input::Keyboard::RightAlt;
		case SDLK_LMETA:
		case SDLK_LSUPER:
			return Input::Keyboard::LeftSystem;
		case SDLK_RMETA:
		case SDLK_RSUPER:
			return Input::Keyboard::RightSystem;
		case SDLK_PAUSE:
			return Input::Keyboard::Pause;
	}
	return -1;
}

void createEventsFromSDLEvent(std::vector<Input::Event> & events, SDL_Event const& sdlEvent)
{
	Input::Event event;
	SDL_GetMouseState(&event.cursorPosition[0], &event.cursorPosition[1]);
	if(sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP)
	{
		event.device = Input::Device::Keyboard;
		event.axis = sdlKeyToKeyAxis(sdlEvent.key.keysym.sym);
		if(event.axis == -1)
		{
			return;
		}
		event.value = (sdlEvent.type == SDL_KEYDOWN ? 1 : 0);
		events.push_back(event);
		if(sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.unicode != 0)
		{
			event.axis = Input::Keyboard::Char;
			event.value = sdlEvent.key.keysym.unicode;
			events.push_back(event);
		}
	}
	else if(sdlEvent.type == SDL_MOUSEMOTION) // generate value changes for both moving and stopping
	{
		if(!gFirstMouseMoveEvent) // the first mouse motion event always gives absolute values in its xrel and yrel, and so is invalid
		{
			event.device = Input::Device::Mouse;
			if(sdlEvent.motion.xrel != 0)
			{
				event.axis = Input::Mouse::X;
				event.value = sdlEvent.motion.xrel;
				events.push_back(event);
				event.value = 0;
				events.push_back(event);
			}
			if(sdlEvent.motion.yrel != 0)
			{
				event.axis = Input::Mouse::Y;
				event.value = sdlEvent.motion.yrel;
				events.push_back(event);
				event.value = 0;
				events.push_back(event);
			}
			gCursorPositionValid = true;
		}
		else
		{
			gFirstMouseMoveEvent = false;
		}
	}
	else if(sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP)
	{
		event.device = Input::Device::Mouse;
		if(sdlEvent.button.button == SDL_BUTTON_WHEELDOWN || sdlEvent.button.button == SDL_BUTTON_WHEELUP)
		{
			if(sdlEvent.type == SDL_MOUSEBUTTONDOWN) // wheel move generates both buttonup and buttondown events, so only keep the buttondown ones
			{
				event.axis = Input::Mouse::Wheel;
				event.value = (sdlEvent.button.button == SDL_BUTTON_WHEELDOWN ? -1 : +1);
				events.push_back(event);
			}
		}
		else
		{
			switch(sdlEvent.button.button)
			{
				case SDL_BUTTON_LEFT:
					event.axis = Input::Mouse::Left; break;
				case SDL_BUTTON_MIDDLE:
					event.axis = Input::Mouse::Middle; break;
				case SDL_BUTTON_RIGHT:
					event.axis = Input::Mouse::Right; break;
				default:
					event.axis = Input::Mouse::Button0 + sdlEvent.button.button;
			}
			event.value = (sdlEvent.type == SDL_MOUSEBUTTONDOWN ? 1 : 0);
			events.push_back(event);
		}
	}
	else if(sdlEvent.type == SDL_JOYAXISMOTION)
	{
		event.device = Input::Device::Controller0 + sdlEvent.jaxis.which;
		event.axis = sdlEvent.jaxis.axis;
		event.value = sdlEvent.jaxis.value;
		events.push_back(event);
	}
	else if(sdlEvent.type == SDL_JOYBALLMOTION)
	{
		event.device = Input::Device::Controller0 + sdlEvent.jaxis.which;
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
		event.device = Input::Device::Controller0 + sdlEvent.jaxis.which;
		Uint8 prevHatValue = gHatValues[sdlEvent.jaxis.which][sdlEvent.jhat.hat];
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
		gHatValues[sdlEvent.jaxis.which][sdlEvent.jhat.hat] = sdlEvent.jhat.value;
	}
	else if(sdlEvent.type == SDL_JOYBUTTONDOWN || sdlEvent.type == SDL_JOYBUTTONUP)
	{
		event.device = Input::Device::Controller0 + sdlEvent.jaxis.which;
		event.axis = 768 + sdlEvent.jbutton.button;
		event.value = (sdlEvent.type == SDL_JOYBUTTONDOWN ? 1 : 0);
		events.push_back(event);
	}
}

void startupInput()
{
	setupKeyToNameMap();
    Input::refreshControllers();
}

void shutdownInput()
{
    for(unsigned int i = 0; i < gJoysticks.size(); ++i)
    {
        SDL_JoystickClose(gJoysticks[i]);
    }
}

std::string Input::Event::toString(int device, int axis)
{
	if(device == Device::Keyboard)
	{
		auto it = gKeyToNameMap.find(axis);
		if(it != gKeyToNameMap.end())
		{
			return it->second + " key";
		}
	}
	else if(device == Device::Mouse)
	{
		if(axis == Mouse::X)
		{
			return "mouse x-axis";
		}
		else if(axis == Mouse::Y)
		{
			return "mouse y-axis";
		}
		else if(axis == Mouse::Wheel)
		{
			return "mouse wheel";
		}
		else if(axis == Mouse::Left)
		{
			return "left mouse button";
		}
		else if(axis == Mouse::Middle)
		{
			return "middle mouse button";
		}
		else if(axis == Mouse::Right)
		{
			return "right mouse button";
		}
		else
		{
			return "mouse button " + std::to_string(axis - Mouse::Button0 + 1);
		}
	}
	else if(device == Device::Controller0)
	{
		if(axis < Controller::Button0)
		{
			return "controller axis " + std::to_string(axis - Controller::Axis0 + 1);
		}
		else
		{
			return "controller button " + std::to_string(axis - Controller::Button0 + 1);
		}
	}

	return std::string();
}

int Input::getNumControllers()
{
	return (int)gJoysticks.size();
}

void Input::refreshControllers()
{
	if(SDL_WasInit(SDL_INIT_EVERYTHING) & SDL_INIT_JOYSTICK)
	{
		for(int i = 0; i < (int)gJoysticks.size(); ++i)
		{
			SDL_JoystickClose(gJoysticks[i]);
		}
		SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
	}
	if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) == 0)
	{
		int nugJoysticks = SDL_NumJoysticks();
		for(int i = 0; i < nugJoysticks; ++i)
		{
			SDL_Joystick * joystick = SDL_JoystickOpen(i);
			if(joystick == NULL)
			{
				throw std::runtime_error("Could not initialize input controller " + std::to_string(i + 1));
			}
			std::vector<Uint8> hatValue;
			int numHats = SDL_JoystickNumHats(joystick);
			for(int i = 0; i < numHats; ++i)
			{
				hatValue.push_back(SDL_HAT_CENTERED);
			}
			gJoysticks.push_back(joystick);
			gHatValues.push_back(hatValue);
		}
	}
}

bool Input::hasValidCursorPosition()
{
	return gCursorEnabled && gCursorPositionValid;
}

Vector2i Input::getCursorPosition()
{
	assert(gCursorEnabled && gCursorPositionValid);
	return gCursorPosition;
}

bool Input::isCursorVisible()
{
	return gCursorVisible;
}

void Input::setCursorVisible (bool visible)
{
	gCursorVisible = visible;
	if(gCursorEnabled)
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

bool Input::isCursorEnabled()
{
	return gCursorEnabled;
}

void Input::setCursorEnabled(bool enabled)
{
	gCursorEnabled = enabled;
	if(enabled)
	{
		if(gCursorVisible)
		{
			SDL_ShowCursor(SDL_ENABLE);
		}
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_WarpMouse((Uint16)gCursorPosition[0], (Uint16)gCursorPosition[1]);
	}
	else
	{
		SDL_ShowCursor(SDL_DISABLE);
		SDL_WM_GrabInput(SDL_GRAB_ON);
	}
}
