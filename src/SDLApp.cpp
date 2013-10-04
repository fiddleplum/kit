#include "App.h"
#include "Input.h"
#include "OpenGL.h"
#include "Gui\Widget.h"
#include "..\..\SDL2-2.0.0\include\SDL.h"
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <fstream>

namespace App
{
	void startupInput();
	void shutdownInput();
	void setCursorPositionValid(bool valid);
	void handleEvent(Event const & event);
	void handleSDLInputEvent(SDL_Event const & sdlEvent);

	SDL_Window * window = nullptr;
	SDL_GLContext glContext = nullptr;

	bool running = false;
	float lastTime = 0.0f;
	std::shared_ptr<Widget> widget;

	ResourceManager<Texture> textureManager;
	ResourceManager<Shader> shaderManager;

	/*** Internal functions ***/

	void handleEvent(Event const & event)
	{
		if(widget != nullptr)
		{
			widget->handleEvent(event);
		}
	}

	void handleSDLEvents()
	{
		// Get SDL Events
		std::vector<SDL_Event> sdlEvents;
		SDL_PumpEvents();
		do
		{
			SDL_Event sdlEvent;
			if(SDL_PollEvent(&sdlEvent) == 0)
			{
				break;
			}
			sdlEvents.push_back(sdlEvent);
		} while(true);

		// Process the SDL events.
		for(SDL_Event const & sdlEvent : sdlEvents)
		{
			if(sdlEvent.type == SDL_QUIT)
			{
				quit();
			}
			else if(sdlEvent.type == SDL_WINDOWEVENT)
			{
				switch(sdlEvent.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE:
						quit();
						break;
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
						if(widget != nullptr)
						{
							widget->setMaxSize(getSize());
						}
						break;
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					case SDL_WINDOWEVENT_LEAVE:
						setCursorPositionValid(false);
						break;
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_ENTER:
						setCursorPositionValid(true);
						break;
				}
			}
			else
			{
				handleSDLInputEvent(sdlEvent);
			}
		}
	}

	void render()
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		Vector2i windowSize = getSize();
		glViewport(0, 0, windowSize[0], windowSize[1]);
		if(widget != nullptr)
		{
			widget->render();
		}
		SDL_GL_SwapWindow(window);
	}

	void doLoop()
	{
		running = true;
		lastTime = SDL_GetTicks() / 1000.0f;
		while(running)
		{
			float newTime = SDL_GetTicks() / 1000.0f;
			float deltaTime = newTime - lastTime; // calculate the last frame's duration
			lastTime = newTime;
			if(running)
			{
				handleSDLEvents();
			}
			if(running)
			{
				if(widget != nullptr)
				{
					widget->handleEvent(UpdateEvent());
				}
			}
			if(running)
			{
				render();
			}
		}
	}

	/*** Header functions ***/

	void showMessage(std::string const & text)
	{
		std::fstream log ("log.txt", std::ios::out);
		log << text;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", text.c_str(), window);
	}

	void setTitle(std::string const & title)
	{
		SDL_SetWindowTitle(window, title.c_str());
	}

	bool isFullscreen()
	{
		return (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
	}

	Vector2i getSize()
	{
		Vector2i size;
		SDL_GetWindowSize(window, &size[0], &size[1]);
		return size;
	}

	void setFullscreen()
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	void setWindowed(Vector2i size)
	{
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowSize(window, size[0], size[1]);
	}

	void quit()
	{
		running = false;
	}

	void setWidget(std::shared_ptr<Widget> widget)
	{
		App::widget = widget;
		if(widget != nullptr)
		{
			widget->setMaxSize(getSize());
		}
	}

	float getTime()
	{
		return (float)SDL_GetTicks() / 1000.0f;
	}

	ResourceManager<Texture> & getTextureManager()
	{
		return textureManager;
	}

	ResourceManager<Shader> & getShaderManager()
	{
		return shaderManager;
	}
}

int main(int argc, char *argv[])
{
    // Grab the params.
	std::vector<std::string> params;
	for(int i = 1; i < argc; ++i)
	{
		params.push_back(std::string(argv[i]));
	}
    
    // Initialize SDL.
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) == -1)
    {
        App::showMessage(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
		return -1;
    }

	// Create the window and renderer.
	App::window = SDL_CreateWindow("Untitled", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if(App::window == NULL)
	{
		App::showMessage(std::string("Failed to create the SDL window:  ") + SDL_GetError() + ". ");
		return -1;
	}
	App::glContext = SDL_GL_CreateContext(App::window);
	glInitialize();
	App::startupInput();

    // Run the user onStartup function.
	try
	{
		App::onStartup(params); // calls user-defined function
	}
	catch(std::runtime_error const & err)
	{
		App::showMessage(err.what());
	}

	// Do the loop.
	App::doLoop();

    // Run the user onShutdown function.
	App::widget.reset();
	try
	{
		App::onShutdown(); // calls user-defined function
	}
	catch(std::runtime_error const & err)
	{
		App::showMessage(err.what());
	}

    // Close window.
	App::shutdownInput();
	SDL_GL_DeleteContext(App::glContext);
	SDL_DestroyWindow(App::window);
    SDL_Quit();

	return 0;
}

