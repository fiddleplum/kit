#include "app.h"
#include "start_finish.h"
#include "../external/SDL2-2.0.0/include/SDL.h"

// Called by SDL to run the entire application.
int main (int argc, char *argv[])
{
	try
	{
		// Grab the params.
		std::vector<std::string> parameters;
		for(int i = 1; i < argc; ++i)
		{
			parameters.push_back(std::string(argv[i]));
		}

		kit::app::initialize();

		kit::start(parameters);

		kit::app::loop();

		kit::finish();

		kit::app::finalize();
	}
	catch(std::runtime_error const & e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
		return -1;
	}
	catch(std::exception)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!", "The application encountered something so horrendous that it died instantly!", nullptr);
		return -1;
	}

	return 0;
}

