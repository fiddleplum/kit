#pragma once

#include <string>
#include <vector>
#include "Vector.h"
#include "Input.h"

/**
This is a simple wrapper for SDL. It provides windowing, input, and OpenGL capabilities with very little setup effort. Just implement the
five functions below, and you're ready to go!

Here is a simple onEntry template. You can put your own initialization and deinitialization stuff inbetween the function calls.

void App::onEntry (std::vector<std::string> const& params)
{
	App::setTitle("New Application");
	App::setSize(false, Vector2i(800, 600));
	App::startLoop();
}
**/

namespace App
{
	/******* Implement these! *******/

	// This is the first function that is called (like the main function). The params are the given at the console, separated by spaces.
	void onEntry(std::vector<std::string> const& params);

	// This is called whenever an input event occurs (can be more than once per loop iteration).
	void onInputEvent(Input::Event event);

	// This is called every time the program loops (after all onEvent calls for that frame). deltaTime is how much time has passed since the last onFrame
	// and is in seconds.
	void onFrame(float deltaTime);

	// This is called every time the program loops (after onFrame). Draw stuff here using OpenGL. The OpenGL state is mostly reset before this is called.
	void onRender();

	// This is called whenever the window resizes (typically used to correct camera aspect ratios or reload textures). For fullscreen, size is the resolution.
	void onResize(Vector2i size);

	/******* You can call these *******/

	// This shows a system message. If in Windows, it opens a popup window, otherwise it prints to stdout.
	void showMessage(std::string const& text);

	/** Window Functions **/

	// The window has a title that you specify. The window can also be fullscreen, in which the resolution is specified by size.
    // The window defaults to a title "Untitled" and in 800x600 windowed mode.
	void setTitle(std::string const& title);
	Vector2i getSize();
	bool isFullscreen();
	void setSize(bool fullscreen, Vector2i size);

	/** Loop Functions **/

	// The loop runs as often as possible. startLoop is typically called from onEntry. Call quit whenever you want to exit the loop.
	void startLoop();
	void quit();

	/** Video Functions **/

	// Retrieves a list of all possible resolutions available to the primary monitor (multiple monitors not yet supported).
	std::vector<Vector2i> getAllResolutions();
}

