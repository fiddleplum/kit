#pragma once

#ifdef _DEBUG
#pragma comment (lib, "base_d.lib")
#pragma comment (lib, "app_d.lib")
#else
#pragma comment (lib, "base.lib")
#pragma comment (lib, "app.lib")
#endif
#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")

#include "list.h"
#include "ptr.h"
#include "pimpl.h"
#include "window.h"
#include "scene.h"

class App : public Pimpl<App>
{
public:
	// Called after the app is opened. Implemented by the user.
	static void onOpen(std::vector<std::string> const & params);

	// Called before the app is closed. Implemented by the user.
	static void onClose();

	// Tells the app that you will be using one of the optional systems. Call it in onOpen. If you don't call this, the system won't function.
	enum { ControllerSystem, AudioSystem };
	static void useSystem(int system);

	// Quits the application. Stops the application loop.
	void quit();

	// Creates a new window.
	Ptr<Window> addWindow(char const * title);

	// Removes a window.
	void removeWindow(Ptr<Window> window);

	// Adds a scene.
	Ptr<Scene> addScene();

	// Removes a scene.
	void removeScene(Ptr<Scene> scene);

	// Shows a message dialog box.
	void showMessage(std::string const & message);

	// Internal. Constructs the app.
	App(std::vector<std::string> const & args);

	// Internal. Destructs the app.
	~App();

	// Internal. Starts the application loop.
	void loop();

	// Internal. Handles an event, passing to the windows and scenes.
	static void handleEvent(Event const & event);
};

