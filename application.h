#pragma once

#include <string>
#include <vector>

/**
This is a simple wrapper for SDL. It provides windowing, input, and OpenGL capabilities with very little setup effort. Just implement the
five functions below, and you're ready to go!

Here is a simple onEntry template. You can put your own initialization and deinitialization stuff inbetween the function calls.

void App::onEntry (std::vector<std::string> const& params)
{
	App::openWindow();
	App::setTitle("New Application");
	App::setSize(false, App::Coord(800, 600));
	App::startLoop();
	App::closeWindow();
}
**/

namespace App
{
	struct Coord
	{
		Coord(int x, int y) : x(x), y(y) {}
		int x;
		int y;
	};

	struct InputEvent
	{
		int device;
		int axis;
		int value;
	};

	/******* Implement these! *******/

	// This is the first function that is called (like the main function). The params are the given at the console, separated by spaces.
	void onEntry(std::vector<std::string> const& params);

	// This is called whenever an input event occurs (can be more than once per loop iteration).
	void onInputEvent(InputEvent const& event);

	// This is called every time the program loops (after all onEvent calls for that frame). deltaTime is how much time has passed since the last onFrame
	// and is in seconds.
	void onFrame(float deltaTime);

	// This is called every time the program loops (after onFrame). Draw stuff here using OpenGL. The OpenGL state is mostly reset before this is called.
	void onRender();

	// This is called whenever the window resizes (typically used to correct camera aspect ratios or reload textures). For fullscreen, size is the resolution.
	void onResize(Coord const& size);

	/******* You can call these *******/

	// This shows a system message. If in Windows, it opens a popup window, otherwise it prints to stdout.
	void showMessage(std::string const& text);

	/** Window Functions **/

	// The window has a title that you specify. The window can also be fullscreen, which the resolution is specified by window_size. There
	// may be only one window open at a time. The window starts with a title "Untitled" and in 800x600 windowed.
	void openWindow();
	void closeWindow();
	void setTitle(std::string const& title);
	Coord getSize();
	bool isFullscreen();
	void setSize(bool fullscreen, Coord windowSize);

	/** Loop Functions **/

	// The loop runs as often as possible. startLoop is typically called from onEntry. Call quit whenever you want to exit the loop.
	void startLoop();
	void quit();

	/** Video Functions **/

	// Retrieves a list of all possible resolutions available to the primary monitor (multiple monitors not yet supported).
	std::vector<Coord> getAllResolutions();

	/** Input Device Functions **/

	// The input types can be a keyboard, a mouse, and a number of controllers. Each device is numbered from 0 (keyboard) to
	// numInputDevices. Use the deviceName and axisName functions for a user-friendly name. If someone plugs in or removes a controller,
	// you can call refreshInputDevices to update the device list. A note about mouseX and mouseY input events: If the cursor is at
	// the edge of the screen and is enabled, the events may not get called. Disable the cursor (see below) to make sure the events
	// always get through, which is ideal for an fps.
	int getNumInputDevices();
	std::string getDeviceName(int device);
	std::string getAxisName(int device, int axis);
	void refreshInputDevices();

	/** Cursor Functions **/

	// The cursor only has a valid position when it is within the window and enabled. ALWAYS call cursorHasValidPosition before calling
	// cursorPosition to avoid unpleasant assert fails (in debug mode). You can also make the system cursor invisible so that you can use
	// your own graphical cursor, if you wish.
	bool hasValidCursorPosition();
	Coord getCursorPosition();
	bool isCursorVisible();
	void setCursorVisible(bool visible);
	bool isCursorEnabled();
	void setCursorEnabled(bool enabled);

	/** Device and Axis Types **/

	// Keys and buttons have a pressed value of 1, otherwise 0. Mouse axes and wheels and joystick axes, balls, and hats have
	// positive values to the right and up. To get controller indices use (event.device - Device::CONTROLLER0). To get extra mouse button axes
	// use (event.axis - MouseAxis::BUTTON0). For characters (including unicode), an event occurs with the axis equal to
	// KeyboardAxis::CHAR and the value equal to the unicode character.
	enum Device
	{
		KEYBOARD, MOUSE, CONTROLLER_0
	};
	enum Mouse
	{
		X, Y, WHEEL, LEFT, MIDDLE, RIGHT, BUTTON_0 = LEFT
	};
	enum Key
	{
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		N_0, N_1, N_2, N_3, N_4, N_5, N_6, N_7, N_8, N_9,
		UP, DOWN, LEFT, RIGHT, PAGE_UP, PAGE_DOWN, HOME, END, ENTER, BACKSPACE, INSERT, DELETE, TAB, SPACE, ESCAPE,
		KP_0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9, KP_ADD, KP_SUBTRACT, KP_MULTIPLY, KP_DIVIDE, KP_DECIMAL, KP_ENTER,
		GRAVE, APOSTROPHE, SEMICOLON, COMMA, PERIOD, SLASH, L_BRACKET, R_BRACKET, BACKSLASH, HYPHEN, EQUALS,
		CAPSLOCK, L_SHIFT, R_SHIFT, L_CTRL, R_CTRL, L_ALT /* SAME AS OPTION */, R_ALT /* SAME AS ALT GR */, L_SYSTEM, R_SYSTEM, // SAME AS WINDOWS OR COMMAND OR META
		F_1, F_2, F_3, F_4, F_5, F_6, F_7, F_8, F_9, F_10, F_11, F_12, F_13, F_14, F_15,
		CHAR
	};
}

