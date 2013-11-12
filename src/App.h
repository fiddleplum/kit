#pragma once

#include "ResourceManager.h"
#include "Vector.h"
#include "Event.h"
#include <string>
#include <vector>
#include <memory>

namespace Gui
{
	class Widget;
}

class Texture;
class Shader;

namespace App
{
	/******* Implement *******/

	/// This function is called just before the loop. The params are the given at the command prompt.
	void onStartup(std::vector<std::string> const & params);

	/// This function is called just after the loop.
	void onShutdown();

	/******* Use *******/

	/// This shows a system message. If in Windows, it opens a popup window, otherwise it prints to stdout.
	void showMessage(std::string const & text);

	/// Sets the title of the window. The window starts with "Untitled".
	void setTitle(std::string const & title);

	/// Returns true if the application is fullscreen. False if windowed.
	bool isFullscreen();

	/// Returns the size of the window, which is the resolution of the screen if fullscreen.
	Vector2i getSize();

	/// Sets the application to be fullscreen.
	void setFullscreen();

	/// If it was in fullscreen, exists fullscreen. Sets the window size. The window starts in 800x600 windowed mode.
	void setWindowed(Vector2i size);

	/// Call quit whenever you want to exit the loop.
	void quit();

	/// Sets the top-level widget. Removes any previous widget.
	void setWidget(std::shared_ptr<Gui::Widget> widget);

	/// Gets the time in seconds since some arbitrary time after the application started, up to 49 days.
	float getTime();

	ResourceManager<Texture> & getTextureManager();

	ResourceManager<Shader> & getShaderManager();

}

