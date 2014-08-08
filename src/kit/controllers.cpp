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
				log::write("Controller " + std::to_string(i));
				log::write("  Num Axes: " + std::to_string(SDL_JoystickNumAxes(joystick)));
				log::write("  Num Buttons: " + std::to_string(SDL_JoystickNumButtons(joystick)));
				log::write("  Num Balls: " + std::to_string(SDL_JoystickNumBalls(joystick)));
				log::write("  Num Hats: " + std::to_string(SDL_JoystickNumHats(joystick)));
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

		void refresh()
		{
			finalize();
			initialize();
		}
	}
}