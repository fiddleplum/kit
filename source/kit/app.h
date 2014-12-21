#pragma once

#include "resource_cache.h"
#include "list.h"

class Cursor;
class Window;
class Scene;

class App
{
public:
	// Construct the application. Calling it more than once before another destroy doesn't do anything.
	static void create(bool usingAudio);

	// Destroy the application. Calling it more than once before another create doesn't do anything.
	static void destroy();

	// Get the singleton App.
	static Ptr<App> get();

	// Runs the application. Starts the application loop, and does not leave the function until quit is called.
	void run();

	// Quits the application. Stops the application loop.
	void quit();

	// Adds a new window.
	Ptr<Window> addWindow(char const * title);

	// Removes a window.
	void removeWindow(Ptr<Window> window);

	// Adds a scene.
	Ptr<Scene> addScene();

	// Removes a scene.
	void removeScene(Ptr<Scene> scene);

	Ptr<Cursor> getCursor();

	// Shows a message dialog box.
	void showMessage(std::string const & message);

	// Gets the resource cache used by the guis and scenes.
	Ptr<ResourceCache> getResourceCache();

private:
	App(bool usingAudio);
	~App();

	bool running = false;
	void * glContext = nullptr;
	OwnPtr<ResourceCache> resourceCache;
	List<Window> windows;
	List<Scene> scenes;
	OwnPtr<Cursor> cursor;
};

