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

//#include "window.h"
//#include "scene.h"
//

#include "ptr.h"
#include <vector>
#include <string>

class Window;
class Scene;

class App
{
public:
	// Returns the single instance of the app.
	static Ptr<App> instance();

	// Called after the app is opened. Implemented by the user.
	static void onOpen(std::vector<std::string> const & params);

	// Called before the app is closed. Implemented by the user.
	static void onClose();

	// Tells the App that you will be using one of the optional systems. Call it in onOpen. If you don't call this, the system won't function.
	enum System { AudioSystem };
	static void useSystem(System system);

	// Quits the application. Stops the application loop.
	static void quit();

	// Creates a new window.
	static Ptr<Window> addWindow(char const * title);

	// Removes a window.
	static void removeWindow(Ptr<Window> window);

	// Adds a scene.
	static Ptr<Scene> addScene();

	// Removes a scene.
	static void removeScene(Ptr<Scene> scene);

	// Shows a message dialog box.
	static void showMessage(std::string const & message);
};

