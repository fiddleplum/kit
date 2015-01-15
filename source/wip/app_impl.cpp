#include "app_impl.h"
#include "open_gl.h"
#include "input_system.h"
#include "texture_cache.h"
#include "shader_cache.h"
#include "scene_model_cache.h"
#include <SDL.h>

#include "audio.h"

Ptr<Window> getWindowFromId(unsigned int id);

void App::useSystem(System system)
{
	switch(system)
	{
		case AudioSystem:
			SDL_InitSubSystem(SDL_INIT_AUDIO);
			break;
		default:
			throw std::runtime_error("Invalid system used");
	}
}

AppImpl::AppImpl(std::vector<std::string> const & args)
{
	glContext = nullptr;
	looping = false;
	targetFrameRate = 60.f;

	// Start SDL.
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
	{
		throw std::runtime_error(std::string("Could not initialize SDL:	") + SDL_GetError() + ". ");
	}

	// Initialize the singletons.
	InputSystem::createInstance();
	TextureCache::createInstance();
	ShaderCache::createInstance();
	SceneModelCache::createInstance();

	// Call the user function.
	App::onOpen(args);
}

AppImpl::~AppImpl()
{
	// Call the user function.
	App::onClose();

	// Destroy the singletons.
	TextureCache::destroyInstance();
	ShaderCache::destroyInstance();
	SceneModelCache::destroyInstance();
	InputSystem::destroyInstance();

	// Stop SDL.
	SDL_Quit();
}

void AppImpl::loop()
{
	app->looping = true;
	while(app->looping)
	{
		float frameStartTime = getTime();

		// Handle events
		controllers::startFrame();
		SDL_Event sdlEvent;
		while(_looping && SDL_PollEvent(&sdlEvent))
		{
			handleSDLEvent(sdlEvent);
		}
		if(!_looping)
		{
			break;
		}
		for(int i = 0; i < controllers::getNumControllers(); i++)
		{
			std::vector<std::pair<int, float>> controllerAxisEvents = controllers::getAxesChangedSinceLastFrame(i);
			for(auto controllerAxisEvent : controllerAxisEvents)
			{
				ControllerAxisEvent event{Ptr<Window>()};
				event.controller = i;
				event.axis = controllerAxisEvent.first;
				event.value = controllerAxisEvent.second;
				handleEvent(event);
			}
		}
		if(!_looping)
		{
			break;
		}

		// Update
		for(auto window : app->windows)
		{
			if(_looping)
			{
				UpdateEvent event = UpdateEvent(window);
				event.dt = 1.f / targetFrameRate;
				window->handleEvent(event);
			}
		}
		if(!_looping)
		{
			break;
		}
		for(auto scene : app->scenes)
		{
			UpdateEvent event = UpdateEvent(Ptr<Window>());
			event.dt = 1.f / targetFrameRate;
			scene->handleEvent(event);
			if(!_looping)
			{
				break;
			}
		}
		if(!looping)
		{
			break;
		}

		// PreRender Update
		for(auto scene : app->scenes)
		{
			scene->handleEvent(PreRenderUpdateEvent(Ptr<Window>()));
		}

		// Render
		for(auto window : app->windows)
		{
			if(looping)
			{
				glSetWindowForRendering(window);
				window->render();
			}
		}

		// FIX THIS: Introduce better loop timing
		float delayTime = (1.f / targetFrameRate) - (getTime() - frameStartTime);
		if(delayTime > 0)
		{
			SDL_Delay((unsigned int)(delayTime * 1000));
		}
	}
}

void quit()
{
	looping = false;
}

Ptr<Window> AppImpl::addWindow(char const * title)
{
	OwnPtr<Window> window;
	window.create(title);
	if(windows.empty())
	{
		glContext = SDL_GL_CreateContext(window->getSDLWindow());
		glInitialize();
	}
	windows.insert(window);
	return window;
}

void AppImpl::removeWindow(Ptr<Window> window)
{
	if(!scenes.empty() && windows.size() == 1 && *windows.begin() == window)
	{
		throw std::runtime_error("All scenes must be removed before the last window is removed.");
	}
	windows.erase(window);
	if(windows.empty())
	{
		SDL_GL_DeleteContext(glContext);
		glContext = 0;
		quit();
	}
}

Ptr<scene::Scene> addScene()
{
	if(windows.empty())
	{
		throw std::runtime_error("A scene may not be created until a window has been created.");
	}
	OwnPtr<scene::Scene> scene;
	scene.create();
	scenes.insert(scene);
	return scene;
}

void removeScene(Ptr<scene::Scene> scene)
{
	_scenes.erase(scene);
}

void showMessage(std::string const & message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information", message.c_str(), nullptr);
}

void App::_setCursorPosition(int windowId, Coord2i position)
{
	Ptr<Window> window = getWindowFromId(windowId);
	if(window.isValid())
	{
		window->setCursorPosition(position);
	}
}

void setCursorActive(bool)
{
	// TODO
}

Ptr<Window> getWindowFromId(unsigned int id)
{
	SDL_Window * sdlWindow = SDL_GetWindowFromID(id);
	if(sdlWindow != NULL)
	{
		for(auto window : windows)
		{
			if(sdlWindow == window->getSDLWindow())
			{
				return window;
			}
		}
	}
	return Ptr<Window>();
}

void App::handleEvent(Event const & event)
{
	for(auto window : app->windows)
	{
		window->handleEvent(event);
	}
	for(auto scene : app->scenes)
	{
		scene->handleEvent(event);
	}
}

void handleSDLEvent(SDL_Event const & sdlEvent)
{
	Ptr<Window> window;
	switch(sdlEvent.type)
	{
		case SDL_QUIT:
			quit();
			break;
		case SDL_WINDOWEVENT:
			window = getWindowFromId(sdlEvent.window.windowID);
			if(!window.isValid())
			{
				return;
			}
			switch(sdlEvent.window.event)
			{
				case SDL_WINDOWEVENT_CLOSE:
					removeWindow(window);
					break;
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					window->setMaxCoord(Coord2i(sdlEvent.window.data1, sdlEvent.window.data2));
					break;
				case SDL_WINDOWEVENT_LEAVE:
					window->getCursor()->setWithinWindow(false);
					break;
				case SDL_WINDOWEVENT_ENTER:
					window->getCursor()->setWithinWindow(true);
					break;
			}
			break;
		default:
			
	}
}

// Called by SDL to run the entire app.
int main(int argc, char *argv[])
{
	try
	{
		// Grab the params.
		std::vector<std::string> args;
		for(int i = 1; i < argc; ++i) // don't include the 0th arg, because it is the program name
		{
			args.push_back(std::string(argv[i]));
		}

		app.create(args);

		app->loop();

		app.destroy();
	}
	catch(std::exception const & e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}

