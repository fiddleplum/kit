#include "main.h"
#include <iostream>
#include <SDL_main.h>
#include <SDL_MessageBox.h>

//extern int entry(std::vector<std::string> const & args);

// Called by SDL to run the entire application.
int main(int argc, char *argv[])
{
#undef main // SDL redefines main, but it interferes with other things.

	int r = 0;

	try
	{
		// Grab the params.
		std::vector<std::string> args;
		for(int i = 1; i < argc; ++i) // don't include the 0th arg, because it is the program name
		{
			args.push_back(std::string(argv[i]));
		}

		// Call the user-supplied main.
		r = kit::main(args);
	}
	catch(std::exception const & e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return r;

	//try
	//{
	//	// Grab the params.
	//	std::vector<std::string> args;
	//	for(int i = 1; i < argc; ++i) // don't include the 0th arg, because it is the program name
	//	{
	//		args.push_back(std::string(argv[i]));
	//	}

	//	kit::app::initialize();

	//	//kit::start(args);

	//	kit::app::loop();

	//	//kit::finish();

	//	kit::app::finalize();
	//}
	//catch(std::runtime_error const & e)
	//{
	//	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
	//	return -1;
	//}
	//catch(std::exception)
	//{
	//	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!", "The application encountered something so horrendous that it died instantly!", nullptr);
	//	return -1;
	//}

	//return 0;
}

