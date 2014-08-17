#include "controllers.h"
#include "log.h"
#include <vector>
#include <string>
#include <SDL.h>

namespace kit
{
	namespace controllers
	{
		std::vector<SDL_Joystick *> joysticks;

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
				SDL_Joystick * joystick = SDL_JoystickOpen(i);
				if(joystick == NULL)
				{
					throw std::runtime_error("Could not initialize input controller " + std::to_string(i + 1));
				}
				joysticks.push_back(joystick);
			}
		}

		void finalize()
		{
			if(SDL_WasInit(SDL_INIT_JOYSTICK) != 0)
			{
				for(int i = 0; i < (int)joysticks.size(); ++i)
				{
					SDL_JoystickClose(joysticks[i]);
				}
				joysticks.clear();
				SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
			}
		}

		int getNumControllers()
		{
			return (int)joysticks.size();
		}

		std::string getName(int controller)
		{
			if(controller < 0 || (int)joysticks.size() <= controller)
			{
				throw std::runtime_error("Invalid controller index.");
			}
			return SDL_JoystickName(joysticks[controller]);
		}

		void refresh()
		{
			finalize();
			initialize();
		}

		int getNumSDLAxes(int joystick)
		{
			return SDL_JoystickNumAxes(joysticks[joystick]);
		}
	}
}