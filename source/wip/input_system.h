#pragma once

#include "coord.h"
#include "singleton.h"
#include <vector>
#include <map>
#include <SDL.h>

class InputSystem : public Singleton<InputSystem>
{
public:
	// Internal. Constructor.
	InputSystem();

	// Internal. Destructor.
	~InputSystem();

	// Internal. Translates an SDL key into an InputKeyboardAxis.
	KeyboardEvent::Key sdlKeyToKey(int sdlKey);

	// Internal. Processes an SDL Event.
	void handleSDLEvent(SDL_Event const & sdlEvent);

	// Internal. Gets a list of the input events since the last poll.
	std::vector<InputEvent> pollEvents();

	// Internal. Gets the input text since the last text poll. Encoded in UTF-8.
	std::string pollText();

	// Internal. Retrieves the cursor position for a window given its SDL id. Returns (0, 0) for invalid windows.
	Coord2i getCursorPosition(SDL_WindowEventID windowId);

private:
	class Controller
	{
	public:
		Controller(int deviceIndex);

		std::string name;
		SDL_Joystick * sdlJoystick;
		std::vector<float> axisStatesLastFrame;
		std::vector<float> axisStates; // SDL axis states
	};

	std::vector<InputEvent> events; // Stores the events since the last event poll.
	std::string text; // Stores the text since the last text poll.
	std::map<SDL_WindowEventID, Coord2i> cursorPositions; // The cursor position of each SDL window.
	std::vector<Controller *> controllers; // Controllers currently connected.
	std::map<SDL_JoystickID, int> sdlJoystickToControllerMapping; // Mapping from the SDL joystick to the controller indices.
};

