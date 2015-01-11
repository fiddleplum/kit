#include "controller_system.h"
#include "event.h"
#include "ptr.h"
#include <vector>
#include <map>
#include <string>
#include <SDL.h>

// Note bug in SDL Code: When querying the joysticks, the indices are reversed compared to when receiving events.


int getNumSDLAxes(int controller)
{
	if(controller < 0 || (int)controllers.size() <= controller)
	{
		return 0;
	}
	return controllers[controller]->axisStates.size();
}

std::vector<OwnPtr<Event>> getEventsFromAxisStates()
{
	std::vector<OwnPtr<Event>> events;
	for(int controllerIndex = 0; controllerIndex < (int)controllers.size(); controllerIndex++)
	{
		for(int axis = 0; axis < (int)controllers[controllerIndex]->axisStates.size(); axis++)
		{
			if(controllers[controllerIndex]->axisStates[axis] != controllers[controllerIndex]->axisStatesLastFrame[axis])
			{
				OwnPtr<ControllerAxisEvent> event;
				event.create();
				event->controller = controllerIndex;
				event->axis = axis;
				event->value = controllers[controllerIndex]->axisStates[axis];
				events.push_back(event);
			}
		}
	}
	return events;
}

std::vector<OwnPtr<Event>> handleSDLJoystickEvent(SDL_Event const & sdlEvent)
{
	std::vector<OwnPtr<Event>> events;
	switch(sdlEvent.type)
	{
		case SDL_JOYAXISMOTION:
		{
			auto it = sdlJoystickToControllerMapping.find(sdlEvent.jdevice.which);
			if(it != sdlJoystickToControllerMapping.end())
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
				controllers[it->second]->axisStates[sdlEvent.jaxis.axis] = value;
			}
			break;
		}
		case SDL_JOYHATMOTION:
		{
			auto it = sdlJoystickToControllerMapping.find(sdlEvent.jdevice.which);
			if(it != sdlJoystickToControllerMapping.end())
			{
				OwnPtr<ControllerAxisEvent> event;
				event.create();
				event->controller = it->second;
				event->axis = controllers[event->controller]->axisStates.size() + sdlEvent.jhat.hat * 2 + 0;
				if((sdlEvent.jhat.value & SDL_HAT_LEFT) != 0)
				{
					event->value = -1;
				}
				else if((sdlEvent.jhat.value & SDL_HAT_RIGHT) != 0)
				{
					event->value = +1;
				}
				else
				{
					event->value = 0;
				}
				events.push_back(event);
				event.create(); // create another event
				event->axis++;
				if((sdlEvent.jhat.value & SDL_HAT_DOWN) != 0)
				{
					event->value = -1;
				}
				else if((sdlEvent.jhat.value & SDL_HAT_UP) != 0)
				{
					event->value = +1;
				}
				else
				{
					event->value = 0;
				}
				events.push_back(event);
				break;
			}
		}
		case SDL_JOYBALLMOTION:
		{
			auto it = sdlJoystickToControllerMapping.find(sdlEvent.jdevice.which);
			if(it != sdlJoystickToControllerMapping.end())
			{
				OwnPtr<ControllerBallEvent> event;
				event.create();
				event->controller = it->second;
				event->ball = sdlEvent.jball.ball;
				event->offset[0] = sdlEvent.jball.xrel;
				event->offset[1] = sdlEvent.jball.yrel;
				events.push_back(event);
				break;
			}
		}
	}
	return events;
}

void ControllerSystem::initialize()
{
}

void ControllerSystem::finalize()
{
	if(SDL_WasInit(SDL_INIT_JOYSTICK) != 0)
	{
		for(int i = 0; i < (int)controllers.size(); ++i)
		{
			SDL_JoystickClose(controllers[i]->sdlJoystick);
			delete controllers[i];
		}
		controllers.clear();
		sdlJoystickToControllerMapping.clear();
		SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
	}
}

int ControllerSystem::getNumControllers()
{
	return (int)controllers.size();
}

std::string ControllerSystem::getName(int controller)
{
	if(controller < 0 || (int)controllers.size() <= controller)
	{
		return "None";
	}
	return controllers[controller]->name;
}

void ControllerSystem::refresh()
{
	finalize();
	initialize();
}

void ControllerSystem::startFrame()
{
	for(auto controller : controllers)
	{
		controller->axisStatesLastFrame = controller->axisStates;
	}
}

void ControllerSystem::updateControllerAxis(int controllerIndex, int axis, float value)
{
	if(controllerIndex < 0 || (int)controllers.size() <= controllerIndex)
	{
		return;
	}
	if(axis < 0 || (int)controllers[controllerIndex]->axisStates.size() <= axis)
	{
		return;
	}
	controllers[controllerIndex]->axisStates[axis] = value;
}

