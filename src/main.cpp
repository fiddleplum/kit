#include "app_internal.h"
#include <kit/start_finish.h>

namespace kit
{
	extern Ptr<AppInternal> appInternal;
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

		kit::OwnPtr<kit::AppInternal> app (new kit::AppInternal);

		kit::appInternal = app;

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

