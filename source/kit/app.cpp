#include "app.h"
#include "open_gl.h"
//#include "input_system.h"
#include "texture_cache.h"
#include "shader_cache.h"
//#include "scene_model_cache.h"
#include "window.h"
#include <SDL.h>

//#include "audio.h"

OwnPtr<App> Singleton<App>::global;

App::App(std::vector<std::string> const & args)
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
	//InputSystem::createInstance();
	TextureCache::createInstance();
	ShaderCache::createInstance();
	//SceneModelCache::createInstance();

	//SDL_InitSubSystem(SDL_INIT_AUDIO);
}

App::~App()
{
	// Destroy the singletons.
	TextureCache::destroyInstance();
	ShaderCache::destroyInstance();
	//SceneModelCache::destroyInstance();
	//InputSystem::destroyInstance();

	// Stop SDL.
	SDL_Quit();
}

void App::quit()
{
	looping = false;
}

Ptr<Window> App::addWindow(std::string const & title)
{
	OwnPtr<Window> window;
	window.setNew(title);
	if(windows.empty())
	{
		glContext = SDL_GL_CreateContext(window->getSDLWindow());
		glInitialize();
	}
	auto it = windows.insert(windows.end(), window);
	windowLookup[window] = it;
	return window;
}

void App::removeWindow(Ptr<Window> window)
{
	//if(!scenes.empty() && windows.size() == 1 && *windows.begin() == window)
	//{
	//	throw std::runtime_error("All scenes must be removed before the last window is removed.");
	//}
	auto it = windowLookup.find(window);
	if(it == windowLookup.end())
	{
		throw std::exception();
	}
	windows.erase(it->second);
	windowLookup.erase(window);
	if(windows.empty())
	{
		SDL_GL_DeleteContext(glContext);
		glContext = 0;
	}
}

//Ptr<scene::Scene> addScene()
//{
//	if(windows.empty())
//	{
//		throw std::runtime_error("A scene may not be created until a window has been created.");
//	}
//	OwnPtr<scene::Scene> scene;
//	scene.create();
//	scenes.insert(scene);
//	return scene;
//}
//
//void removeScene(Ptr<scene::Scene> scene)
//{
//	_scenes.erase(scene);
//}

void App::showMessage(std::string const & message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information", message.c_str(), nullptr);
}

void App::loop()
{
	looping = true;
	while(looping)
	{
		float frameStartTime = SDL_GetTicks() / 1000.f;

		//// Handle events
		//controllers::startFrame();
		SDL_Event sdlEvent;
		while(SDL_PollEvent(&sdlEvent))
		{
			handleSDLEvent(sdlEvent);
		}
		//for(int i = 0; i < controllers::getNumControllers(); i++)
		//{
		//	std::vector<std::pair<int, float>> controllerAxisEvents = controllers::getAxesChangedSinceLastFrame(i);
		//	for(auto controllerAxisEvent : controllerAxisEvents)
		//	{
		//		ControllerAxisEvent event{Ptr<Window>()};
		//		event.controller = i;
		//		event.axis = controllerAxisEvent.first;
		//		event.value = controllerAxisEvent.second;
		//		handleEvent(event);
		//	}
		//}

		// Update
		for(auto & window : windows)
		{
			window->update(1.f / targetFrameRate);
		}
		//for(auto & scene : app->scenes)
		//{
		//	UpdateEvent event = UpdateEvent(Ptr<Window>());
		//	event.dt = 1.f / targetFrameRate;
		//	scene->handleEvent(event);
		//}

		// PreRender Update
		for(auto & window : windows)
		{
			window->preRenderUpdate();
		}
		//for(auto scene : app->scenes)
		//{
		//	scene->handleEvent(PreRenderUpdateEvent(Ptr<Window>()));
		//}

		// Render
		for(auto const & window : windows)
		{
			window->render(glContext);
		}

		// FIX THIS: Introduce better loop timing
		float delayTime = (1.f / targetFrameRate) - (SDL_GetTicks() / 1000.f - frameStartTime);
		if(delayTime > 0)
		{
			SDL_Delay((unsigned int)(delayTime * 1000));
		}
	}
}

void App::handleSDLEvent(SDL_Event const & sdlEvent)
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
					window->handleResize({sdlEvent.window.data1, sdlEvent.window.data2});
					break;
				case SDL_WINDOWEVENT_LEAVE:
					window->setCursorWithinWindow(false);
					break;
				case SDL_WINDOWEVENT_ENTER:
					window->setCursorWithinWindow(true);
					break;
			}
			break;
	}
}

Ptr<Window> App::getWindowFromId(unsigned int id) const
{
	SDL_Window * sdlWindow = SDL_GetWindowFromID(id);
	if(sdlWindow != NULL)
	{
		for(auto const & window : windows)
		{
			if(sdlWindow == window->getSDLWindow())
			{
				return window;
			}
		}
	}
	return Ptr<Window>();
}

//void App::_setCursorPosition(int windowId, Coord2i position)
//{
//	Ptr<Window> window = getWindowFromId(windowId);
//	if(window.isValid())
//	{
//		window->setCursorPosition(position);
//	}
//}
//
//void setCursorActive(bool)
//{
//	// TODO
//}

//void App::handleEvent(Event const & event)
//{
//	for(auto window : app->windows)
//	{
//		window->handleEvent(event);
//	}
//	for(auto scene : app->scenes)
//	{
//		scene->handleEvent(event);
//	}
//}
//

// Called by SDL to run the entire app.
int main(int argc, char *argv[])
{
	std::vector<std::string> args;
	try
	{
		// Grab the params.
		for(int i = 1; i < argc; ++i) // don't include the 0th arg, because it is the program name
		{
			args.push_back(std::string(argv[i]));
		}

		App::createInstance(args);
	}
	catch(std::exception const & e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
		return -1;
	}
	try
	{
		onAppOpen(args);
	}
	catch(std::exception const & e)
	{
		App::destroyInstance();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
		return -1;
	}
	try
	{
		App::instance()->loop();

		onAppClose();

		App::destroyInstance();
	}
	catch(std::exception const & e)
	{
		onAppClose();
		App::destroyInstance();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
		return -1;
	}

	return 0;
}

