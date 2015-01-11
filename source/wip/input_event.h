#pragma once

// Relative axes like mouse motion or track balls generate one event for moving and another for stopping (with value 0).

class InputDevice
{
public:
	enum
	{
		Keyboard, Mouse, Controller0
	};
};

class InputKeyboardAxis
{
public:
	enum
	{
		Unknown,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, // Regular number keys
		Up, Down, Left, Right, PageUp, PageDown,
		Home, End, Enter, Backspace, Insert, Delete, Tab, Space, Escape,
		Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9, // Keypad number keys
		KeypadAdd, KeypadSubtract, KeypadMultiply, KeypadDivide, KeypadDecimal, KeypadEnter,
		Grave, Apostrophe, Semicolon, Comma, Period, Slash,
		LeftBracket, RightBracket, Backslash, Hyphen, Equals,
		Capslock, LeftShift, RightShift, LeftControl, RightControl,
		LeftAlt /*!	same as option */, RightAlt /*! same as alt gr */,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
		Pause
	};
};

class InputMouseAxis
{
public:
	enum
	{
		X,
		Y,
		ScrollX,
		ScrollY,
		Left,
		Middle,
		Right,
		Button0 = Left
	};
};

class InputEvent
{
public:
	int device;
	int axis;
	float value;

	bool isPressed() const
	{
		return value > 0.5f;
	}
};

