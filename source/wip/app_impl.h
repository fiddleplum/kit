#pragma once

#include <kit/app.h>
#include <kit/ptr.h>
#include "list.h"
#include "window.h"
#include "scene.h"

class AppImpl : virtual public App, virtual public Singleton<AppImpl>
{
public:
	// Constructor.
	AppImpl(std::vector<std::string> const & args);

	// Destructor.
	~AppImpl();

	// Starts the application loop.
	static void loop();

	// Handles an event, passing it to the windows and scenes.
	static void handleEvent(Event const & event);

private:
	bool looping;
	float targetFrameRate;
	List<OwnPtr<Window>> windows;
	List<OwnPtr<Scene>> scenes;
	SDL_GLContext glContext;
};

