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
#include "singleton.h"
#include <list>
#include <vector>
#include <string>
#include <map>

class Window;
class Scene;
typedef void * SDL_GLContext;
union SDL_Event;

// Called after the app is opened and before the loop. Implemented by the user.
void onAppOpen(std::vector<std::string> const & params);

// Called before the app is closed. Implemented by the user.
void onAppClose();

class App : public Singleton<App>
{
public:
	// Constructor. Takes commmand line arguments.
	App(std::vector<std::string> const & args);

	// Destructor.
	~App();

	// Quits the application. Stops the loop.
	void quit();

	// Creates a new window.
	Ptr<Window> addWindow(std::string const & title);

	// Removes a window.
	void removeWindow(Ptr<Window> window);

	// Adds a scene.
	//Ptr<Scene> addScene();

	// Removes a scene.
	//void removeScene(Ptr<Scene> scene);

	// Shows a message dialog box.
	void showMessage(std::string const & message);

	// Called by main to start the loop.
	void loop();

private:
	void handleSDLEvent(SDL_Event const & event);
	Ptr<Window> getWindowFromId(unsigned int id) const;

	std::list<OwnPtr<Window>> windows;
	std::map<Ptr<Window>, std::list<OwnPtr<Window>>::iterator> windowLookup;
	bool looping;
	float targetFrameRate;
	SDL_GLContext glContext;
};

