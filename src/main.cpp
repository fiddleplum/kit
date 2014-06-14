#include "app_p.h"
#include <kit/start_finish.h>
#include "../external/SDL2-2.0.0/include/SDL.h"

namespace kit
{
	extern OwnPtr<AppP> gApp;
}

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

		kit::OwnPtr<kit::AppP> app (new kit::AppP);

		kit::gApp = app;

		kit::start(app, parameters);

		app->loop();

		kit::finish();

	}
	catch(std::runtime_error const & e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
		return -1;
	}

	return 0;
}

