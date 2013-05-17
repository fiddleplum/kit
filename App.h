#pragma once

#include <string>
#include <vector>
#include "Vector.h"
#include "Input.h"

/**
This is a simple wrapper for SDL. It provides windowing, input, and OpenGL capabilities with very little setup effort. Just implement the
five functions below, and you're ready to go!
**/

namespace App
{
	/******* Implement *******/

	/// This is the first function that is called (like the main function), even before the loop. The params are the given at the console.
	void onEntry(std::vector<std::string> const& params);

	/// This is called whenever an input event occurs (can be more than once per loop iteration).
	void onInputEvent(Input::Event event);

	/// This is called once every time the program loops (after all onEvent calls for that frame). deltaTime is how much time has passed since the last onFrame and is in seconds.
	void onFrame(float deltaTime);

	/// This is called every time the program loops (after onFrame). Draw stuff here using OpenGL. The OpenGL state is mostly reset before this is called.
	void onRender();

	/// This is called whenever the window resizes (typically used to correct camera aspect ratios or reload textures). For fullscreen, size is the resolution.
	void onResize(Vector2i size);

	/******* Call *******/

	/// This shows a system message. If in Windows, it opens a popup window, otherwise it prints to stdout.
	void showMessage(std::string const& text);

	/// Sets the title of the window. The window starts with "Untitled".
	void setTitle(std::string const& title);
	
	/// Returns true if the application is fullscreen. False if windowed.
	bool isFullscreen();
	
	/// Returns the size of the window, which is the resolution of the screen if fullscreen.
	Vector2i getSize();
	
	/// Sets whether the application is fullscreen, and sets the resolution (if fullscreen) or the window size. The window starts in 800x600 windowed mode.
	void setSize(bool fullscreen, Vector2i size);

	/// Call quit whenever you want to exit the loop.
	void quit();

	/// Retrieves a list of all possible resolutions available to the primary monitor (multiple monitors not yet supported).
	std::vector<Vector2i> getAllResolutions();
}

