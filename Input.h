#pragma once

#include "Vector.h"
#include <string>

/// Used by widgets and others to handle user input.
class Input
{
public:
	class Event
	{
	public:
		/// Returns a user-friendly name of a given input device and axis.
		static std::string toString(int device, int axis);

		int device;
		int axis;
		int value;
		Vector2i cursorPosition;
	};

	/// Controllers/Joysticks plugged in will have a device starting at Controller0 + k, where k is from 0 to one less than the number of controllers plugged in.
	class Device
	{
	public:
		enum
		{
			Keyboard, Mouse, Controller0
		};
	};

	/// Each controller can have multiple of each of the following kinds of input. For example, use "Button0 + i" to get button i on the controller.
	class Controller
	{
	public:
		enum
		{
			Axis0 = 0,
			Button0 = 256
		};
	};

	/// You can use the standard Left/Middle/Right enumerations or use Button0 + k, where k is from 0 to one less than the number of buttons on the mouse. X and Y are always relative to the previous cursor position.
	class Mouse
	{
	public:
		enum
		{
			X, Y, Wheel, Left, Middle, Right, Button0 = Left
		};
	};

	/// Axes of the keyboard.
	class Keyboard
	{
	public:
		enum
		{
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, // Regular number keys
			Up, Down, Left, Right, PageUp, PageDown,
			Home, End, Enter, Backspace, Insert, Delete, Tab, Space, Escape,
			KP0, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9, // Keypad number keys
			KPAdd, KPSubtract, KPMultiply, KPDivide, KPDecimal, KPEnter,
			Grave, Apostrophe, Semicolon, Comma, Period, Slash,
			LBracket, RBracket, Backslash, Hyphen, Equals,
			Capslock, LShift, RShift, LControl, RControl,
			LAlt /*!	same as option */, RAlt /*! same as alt gr */,
			LSystem, RSystem, /// same as windows or command or meta
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
			Pause,
			Char // Use UserInput.value to find the character entered.
		};
	};

	/// Returns the number of input devices, including the mouse and keyboard.
	int getNumInputDevices();

	/// Reloads the controllers, enabling plug-in and play functionality.
	void refreshControllers();

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
	
	/// Enables or disables the cursor. When disabled, the mouse will always return good x and y offsets, which is good for first-person views or 'grabbing' 3D objects.
	void setCursorEnabled(bool enabled);
};

