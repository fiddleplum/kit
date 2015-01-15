#include "input_system.h"
#include "event.h"
#include "app.h"
#include <SDL.h>

// TODO: The first mouse move event may give absolute values for rel, so it may need to be skipped.

InputSystem::InputSystem()
{
	// Initialize the SDL subsystem.
	if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
	{
		throw std::runtime_error("Could not start SDL joystick subsystem.");
	}

	// Initialize the joysticks.
	int numJoysticks = SDL_NumJoysticks();
	for(int deviceIndex = 0; deviceIndex < numJoysticks; deviceIndex++)
	{
		Controller * controller = new Controller(deviceIndex);
		controllers.push_back(controller);
		sdlJoystickToControllerMapping[SDL_JoystickInstanceID(controller->sdlJoystick)] = deviceIndex;
	}
}

InputSystem::~InputSystem()
{
	for(int i = 0; i < (int)controllers.size(); i++)
	{
		SDL_JoystickClose(controllers[i]->sdlJoystick);
		delete controllers[i];
	}
	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

KeyboardEvent::Key InputSystem::sdlKeyToKey(int sdlKey)
{
	if(SDLK_a <= sdlKey && sdlKey <= SDLK_z)
	{
		return (KeyboardEvent::Key)(KeyboardEvent::A + (sdlKey - SDLK_a));
	}
	else if(SDLK_0 <= sdlKey && sdlKey <= SDLK_9)
	{
		return (KeyboardEvent::Key)(KeyboardEvent::N0 + (sdlKey - SDLK_0));
	}
	else if(SDLK_KP_0 <= sdlKey && sdlKey <= SDLK_KP_9)
	{
		return (KeyboardEvent::Key)(KeyboardEvent::Keypad0 + (sdlKey - SDLK_KP_0));
	}
	else if(SDLK_F1 <= sdlKey && sdlKey <= SDLK_F12)
	{
		return (KeyboardEvent::Key)(KeyboardEvent::F1 + (sdlKey - SDLK_F1));
	}
	else if(SDLK_F13 <= sdlKey && sdlKey <= SDLK_F15)
	{
		return (KeyboardEvent::Key)(KeyboardEvent::F13 + (sdlKey - SDLK_F13));
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

void InputSystem::handleSDLEvent(SDL_Event const & sdlEvent)
{
	switch(sdlEvent.type)
	{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			if(sdlEvent.key.repeat != 0)
			{
				break;
			}
			KeyboardEvent event;
			event.key = sdlKeyToKey(sdlEvent.key.keysym.sym);
			event.pressed = (sdlEvent.type == SDL_KEYDOWN ? 1.0f : 0.0f);
			App::handleEvent(event);
			break;
		}
		case SDL_TEXTINPUT:
		{
			text.append(sdlEvent.text.text);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
			if(sdlEvent.button.which == SDL_TOUCH_MOUSEID) // Ignore touch events that simulate mouse for now.
			{
				break;
			}
			InputEvent event;
			event.device = InputDevice::Mouse;
			switch(sdlEvent.button.button)
			{
				case SDL_BUTTON_LEFT:
					event.axis = InputMouseAxis::Left; break;
				case SDL_BUTTON_MIDDLE:
					event.axis = InputMouseAxis::Middle; break;
				case SDL_BUTTON_RIGHT:
					event.axis = InputMouseAxis::Right; break;
				case SDL_BUTTON_X1:
					event.axis = InputMouseAxis::Button0 + 3; break;
				case SDL_BUTTON_X2:
					event.axis = InputMouseAxis::Button0 + 4; break;
			}
			event.value = (sdlEvent.type == SDL_MOUSEBUTTONDOWN ? 1.0f : 0.0f);
			events.push_back(event);
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			if(sdlEvent.button.which == SDL_TOUCH_MOUSEID) // Ignore touch events that simulate mouse for now.
			{
				break;
			}
			InputEvent event;
			event.device = InputDevice::Mouse;
			if(sdlEvent.wheel.x != 0)
			{
				event.axis = InputMouseAxis::ScrollX;
				event.value = (float)sdlEvent.wheel.x;
				events.push_back(event);
			}
			if(sdlEvent.wheel.y != 0)
			{
				event.axis = InputMouseAxis::ScrollY;
				event.value = (float)sdlEvent.wheel.y;
				events.push_back(event);
			}
			break;
		}
		case SDL_MOUSEMOTION: // Generates value changes for both moving and stopping.
		{
			if(sdlEvent.button.which == SDL_TOUCH_MOUSEID) // Ignore touch events that simulate mouse for now.
			{
				break;
			}
			InputEvent event;
			event.device = InputDevice::Mouse;
			if(sdlEvent.motion.xrel != 0)
			{
				event.axis = InputMouseAxis::X;
				event.value = (float)sdlEvent.motion.xrel;
				events.push_back(event);
			}
			if(sdlEvent.motion.yrel != 0)
			{
				event.axis = InputMouseAxis::Y;
				event.value = (float)sdlEvent.motion.yrel;
				events.push_back(event);
			}
			cursorPositions[(SDL_WindowEventID)sdlEvent.motion.windowID].set(sdlEvent.motion.x, sdlEvent.motion.y);
			break;
		}
		case SDL_JOYDEVICEADDED:
		{
			Controller * controller = new Controller(sdlEvent.jdevice.which);
			int i = 0;
			for(; i < (int)controllers.size(); i++) // find an unused controller index
			{
				if(controllers[i]->sdlJoystick == 0)
				{
					delete controllers[i];
					controllers[i] = controller;
					break;
				}
			}
			if(i == (int)controllers.size()) // didn't find one, so add it to the end
			{
				controllers.push_back(controller);
			}
			sdlJoystickToControllerMapping[SDL_JoystickInstanceID(controller->sdlJoystick)] = i;
			break;
		}
		case SDL_JOYDEVICEREMOVED:
		{
			auto it = sdlJoystickToControllerMapping.find(sdlEvent.jdevice.which);
			if(it != sdlJoystickToControllerMapping.end())
			{
				Controller * controller = controllers[it->second];
				sdlJoystickToControllerMapping.erase(it);
				controller->sdlJoystick = 0; // make this controller invalid
			}
			break;
		}
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		{
			auto it = sdlJoystickToControllerMapping.find(sdlEvent.jdevice.which);
			if(it != sdlJoystickToControllerMapping.end())
			{
				OwnPtr<ControllerButtonEvent> event;
				event.create();
				event->controller = it->second;
				event->button = sdlEvent.jbutton.button;
				event->pressed = (sdlEvent.type == SDL_JOYBUTTONDOWN);
				events.push_back(event);
			}
			break;
		}
	}
}

std::string InputSystem::pollText()
{
	std::string polledText = text;
	text.clear();
	return polledText;
}

std::vector<InputEvent> InputSystem::pollEvents()
{
	std::vector<InputEvent> polledEvents = events;
	events.clear();
	return polledEvents;
}

Coord2i InputSystem::getCursorPosition(SDL_WindowEventID windowId)
{
	auto it = cursorPositions.find(windowId);
	if(it != cursorPositions.end())
	{
		return it->second;
	}
	return Coord2i(0, 0);
}

InputSystem::Controller::Controller(int deviceIndex)
{
	name = SDL_JoystickNameForIndex(deviceIndex);
	sdlJoystick = SDL_JoystickOpen(deviceIndex);
	if(sdlJoystick == NULL)
	{
		throw std::runtime_error("Could not initialize controller " + name);
	}
	int numAxes = SDL_JoystickNumAxes(sdlJoystick);
	axisStatesLastFrame.resize(numAxes, 0);
	axisStates.resize(numAxes, 0);
}

