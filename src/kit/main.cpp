#include "app.h"
#include "start_finish.h"
#include "log.h"
#include <SDL.h>

// Called by SDL to run the entire application.
int main (int argc, char *argv[])
{
	kit::log::initialize();

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
		kit::log::write(std::string("Error: ") + e.what());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
		return -1;
	}
	catch(std::exception)
	{
		kit::log::write("Fatal error!");
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!", "The application encountered something so horrendous that it died instantly!", nullptr);
		return -1;
	}

	kit::log::finalize();

	return 0;
}

