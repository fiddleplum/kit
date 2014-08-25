#include "controllers.h"
#include "log.h"
#include <vector>
#include <string>
#include <SDL.h>

namespace kit
{
	namespace controllers
	{
		class Controller
		{
		public:
			std::string name;
			SDL_Joystick * sdlJoystick;
			std::vector<float> axisStatesLastFrame;
			std::vector<float> axisStates;
		};

		std::vector<Controller *> controllers;

		void initialize()
		{
			if(SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
			{
				if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
				{
					throw std::runtime_error("Could not start SDL joystick subsystem.");
				}
			}

			int numJoysticks = SDL_NumJoysticks();
			for(int i = 0; i < numJoysticks; ++i)
			{
				Controller * controller = new Controller;
				controller->sdlJoystick = SDL_JoystickOpen(i);
				if(controller->sdlJoystick == NULL)
				{
					throw std::runtime_error("Could not initialize input controller " + std::to_string(i + 1));
				}
				controller->name = SDL_JoystickNameForIndex(i);
				controller->axisStates.resize(SDL_JoystickNumAxes(controller->sdlJoystick), 0);
				controllers.push_back(controller);
			}
		}

		void finalize()
		{
			if(SDL_WasInit(SDL_INIT_JOYSTICK) != 0)
			{
				for(int i = 0; i < (int)controllers.size(); ++i)
				{
					SDL_JoystickClose(controllers[i]->sdlJoystick);
					delete controllers[i];
				}
				controllers.clear();
				SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
			}
		}

		int getNumControllers()
		{
			return (int)controllers.size();
		}

		std::string getName(int controller)
		{
			if(controller < 0 || (int)controllers.size() <= controller)
			{
				return "None";
			}
			return controllers[getNumControllers() - 1 - controller]->name;
		}

		void refresh()
		{
			finalize();
			initialize();
		}

		int getNumSDLAxes(int controller)
		{
			if(controller < 0 || (int)controllers.size() <= controller)
			{
				return 0;
			}
			return controllers[controller]->axisStates.size();
		}

		void startFrame()
		{
			for(auto controller : controllers)
			{
				controller->axisStatesLastFrame = controller->axisStates;
			}
		}

		void updateControllerAxis(int controllerIndex, int axis, float value)
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

		std::vector<std::pair<int, float>> getAxesChangedSinceLastFrame(int controllerIndex)
		{
			std::vector<std::pair<int, float>> r;
			if(controllerIndex < 0 || (int)controllers.size() <= controllerIndex)
			{
				return r;
			}
			for(int axis = 0; axis < (int)controllers[controllerIndex]->axisStates.size(); axis++)
			{
				if(controllers[controllerIndex]->axisStates[axis] != controllers[controllerIndex]->axisStatesLastFrame[axis])
				{
					r.push_back(std::pair<int, float>(axis, controllers[controllerIndex]->axisStates[axis]));
				}
			}
			return r;
		}
	}
}