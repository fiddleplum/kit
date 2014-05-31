#pragma once

#include "resource_manager.h"
#include <kit/vector.h>
#include <kit/event.h>
#include <string>
#include <vector>
#include <memory>

namespace kit
{
	namespace app
	{
		class Widget;
	}

	class Texture;
	class Shader;

	/// This function is called just before the loop. The params are the given at the command prompt.
	//void onStartup(std::vector<std::string> const & params);

	/// This function is called just after the loop.
	void onShutdown();

	namespace App
	{
		/******* Implement *******/

		/// TODO: CHANGE TO CONTROLLERS -- Called after the widget handles the event.
		void handleAppEvent(Event const & event);

		/******* Messaging and App Control *******/

		/// Gets the time in seconds since some arbitrary time after the application started, up to 49 days.
		float getTime();

		/// This shows a system message. If in Windows, it opens a popup window, otherwise it prints to stdout.
		void showMessage(std::string const & text);

		/// Logs a message.
		void log(std::string const & text);

		/// Call quit whenever you want to exit the loop.
		void quit();

		/******* Window *******/

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

		/******* Widgets *******/

		/// Get the root widget.
		std::shared_ptr<app::Widget> getWidget();

		/// Shortcut for setWidget. Widget must be a app::Widget.
		template <typename Type> std::shared_ptr<Type> setWidget();

		/// Sets the top-level widget. Removes any previous widget.
		void setWidget(std::shared_ptr<app::Widget> widget);

		/******* Cursor *******/

		/// Returns true if the cursor has a valid position (not disabled and within the window).
		bool hasValidCursorPosition();
	
		/// Returns the cursor position. If the cursor does not have a valid position, it fails an assertion.
		Vector2i getCursorPosition();
	
		/// Returns true if the system cursor is visible.
		bool isCursorVisible();
	
		/// Sets the system cursor is visible. This can be used to make a custom OpenGL cursor.
		void setCursorVisible(bool visible);
	
		/// Returns true if the cursor is enabled.
		bool isCursorEnabled();
	
		/// Enables or disables the cursor. When disabled, the mouse will always return good x and y offsets, which is good for first-person views or 'grabbing' 3D objects. May throw runtime_exception.
		void setCursorEnabled(bool enabled);

		/******* Controllers *******/

		/// Returns the number of controllers.
		int getNumControllers();

		/// Reloads the controllers, enabling plug-in and play functionality.
		void refreshControllers();

		/******* Resource Managers *******/

		// Gets the texture manager.
		std::shared_ptr<ResourceManager<Texture>> getTextureManager();

		// Gets the shader manager.
		std::shared_ptr<ResourceManager<Shader>> getShaderManager();
	};

	// Template Implementations

	template <typename Type>
	std::shared_ptr<Type> App::setWidget()
	{
		auto widgetT = std::make_shared<Type>();
		setWidget(widgetT);
		return widgetT;
	}
}

