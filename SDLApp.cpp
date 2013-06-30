#include "App.h"
#include "Texture.h"
#include "Shader.h"
#include <SDL.h>
#include <SDL_opengl.h>
#ifdef __WIN32__
#include <windows.h>
#endif
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <iostream>

void createEventsFromSDLEvent(std::vector<Input::Event> & events, SDL_Event const & sdlEvent);
void startupInput();
void shutdownInput();
extern bool gCursorPositionValid;
extern bool gCursorEnabled;
extern Vector2i gCursorPosition;

namespace App
{
	bool mFullscreen = false;
	Vector2i mWindowSize(800, 600);
	bool mRunning = false;
	float mLastTime = 0.0f;

	/*** Internal functions ***/

	void handleSDLEvents()
	{
		SDL_Event event;
		std::vector<Input::Event> events;
		SDL_PumpEvents();
		while(SDL_PollEvent(&event) == 1)
		{
			if(event.type == SDL_QUIT)
			{
				quit();
			}
			else if(event.type == SDL_VIDEOEXPOSE || event.type == SDL_USEREVENT || event.type == SDL_SYSWMEVENT)
			{ // Do nothing.
			}
			else if(event.type == SDL_VIDEORESIZE)
			{
				if(!mFullscreen)
				{
					Vector2i size(event.resize.w, event.resize.h);
					setSize(mFullscreen, size);
					onResize(mWindowSize); // calls user-defined function
					mLastTime = SDL_GetTicks() / 1000.0f;
				}
			}
			else if(event.type == SDL_ACTIVEEVENT)
			{
				if(mFullscreen && (event.active.type & SDL_APPACTIVE) != 0 && event.active.gain == 0)
				{
					setSize(true, getSize());
				}
				if((event.active.type & SDL_APPMOUSEFOCUS) != 0)
				{
					gCursorPositionValid = (event.active.gain == 1);
				}
			}
			else
			{
				createEventsFromSDLEvent(events, event);
			}
		}
		if(gCursorEnabled)
		{
			SDL_GetMouseState(&gCursorPosition[0], &gCursorPosition[1]);
		}
		for(unsigned int i = 0; i < events.size(); ++i)
		{
			onInputEvent(events[i]); // calls user-defined function
		}
	}

	void render()
	{
		// mostly reset to basic state
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		for(int i = 0; i < GL_MAX_LIGHTS; i++)
		{
			glDisable(GL_LIGHT0 + i);
		}
		glColor4f(1, 1, 1, 1);

		// clear window
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 0, mWindowSize[0], mWindowSize[1]);
		glViewport(0, 0, mWindowSize[0], mWindowSize[1]);
		glClearColor(0, 0, 0, 0);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// reset the matrices
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		onRender(); // calls user-defined function

		SDL_GL_SwapBuffers();
	}

	void doLoop()
	{
		mRunning = true;
		mLastTime = SDL_GetTicks() / 1000.0f;
		while(mRunning)
		{
			float newTime = SDL_GetTicks() / 1000.0f;
			float deltaTime = newTime - mLastTime; // calculate the last frame's duration
			mLastTime = newTime;
			if(mRunning)
			{
				handleSDLEvents();
			}
			if(mRunning)
			{
				onFrame(deltaTime); // calls user-defined function
			}
			if(mRunning)
			{
				render();
			}
		}
	}

	/*** Header functions ***/

	void showMessage(std::string const & text)
	{
	#ifdef __WIN32__
		MessageBoxA(NULL, text.c_str(), "Message", MB_OK);
	#else
		std::cout << text << std::endl;
	#endif
	}

	void setTitle(std::string const & title)
	{
		SDL_WM_SetCaption(title.c_str(), title.c_str());
	}

	bool isFullscreen()
	{
		return mFullscreen;
	}

	Vector2i getSize()
	{
		return mWindowSize;
	}

	void setSize(bool fullscreen, Vector2i windowSize)
	{
		Uint32 flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;
		if(fullscreen)
		{
			flags |= SDL_FULLSCREEN;
		}
		else
		{
			flags |= SDL_ANYFORMAT | SDL_RESIZABLE;
		}
		if(SDL_SetVideoMode(windowSize[0], windowSize[1], 32, flags) == NULL)
		{
			std::stringstream ss;
			ss << "A window size of " << windowSize[0] << "x" << windowSize[1] << " is not supported while in " << (fullscreen ? "fullscreen" : "windowed") << " mode. ";
			throw std::runtime_error(ss.str());
		}
		mWindowSize = windowSize;
		mFullscreen = fullscreen;
	}

	void quit()
	{
		mRunning = false;
	}

	std::vector<Vector2i> getAllResolutions()
	{
		std::vector<Vector2i> resolutions;
		SDL_Rect ** rects = SDL_ListModes(0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN);
		for(int i = 0; rects[i] != 0; ++i)
		{
			resolutions.push_back(Vector2i(rects[i]->w, rects[i]->h));
		}
		return resolutions;
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
    
    // Open the window.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        App::showMessage(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
		return -1;
    }
    SDL_EnableUNICODE(SDL_ENABLE);
    App::setSize(false, Vector2i(800, 600));
	startupInput();

	gTextureManager = new ResourceManager<Texture, std::string>;
    
    // Run the user onEntry function.
	try
	{
		App::onEntry(params); // calls user-defined function
	}
	catch(std::runtime_error const & err)
	{
		App::showMessage(err.what());
	}

	// Do the loop.
	App::doLoop();

	delete gTextureManager;

    // Close window.
	shutdownInput();
    SDL_Quit();

	return 0;
}

