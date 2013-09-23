#pragma once

#include "Vector.h"
#include <string>

class Event
{
public:
	enum Type
	{
		Shutdown, // no class
		Resize,
		Update,
		Key,
		Text,
		MouseButton,
		MouseMove,
		MouseWheel,
		ControllerButton,
		ControllerAxis,
		ControllerBall,
	};

	virtual std::string toString() const = 0;

	Type type;
};

class ShutdownEvent : public Event
{
public:
	virtual std::string toString() const override;
};

class ResizeEvent : public Event
{
public:
	virtual std::string toString() const override;

	Vector2i size; // size of client portion of the window (not including borders/title/etc)
};

class UpdateEvent : public Event
{
public:
	virtual std::string toString() const override;
};

class KeyboardEvent : public Event
{
public:
	enum Key
	{
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

	virtual std::string toString() const override;

	Key key;
	bool pressed;
};

class TextEvent : public Event
{
public:
	virtual std::string toString() const override;

	std::string text; // UTF-8 encoded
};

class MouseButtonEvent : public Event
{
public:
	enum Button
	{
		Left,
		Middle,
		Right
	};

	virtual std::string toString() const override;

	int button;
	bool pressed;
};

class MouseMoveEvent : public Event
{
public:
	virtual std::string toString() const override;

	Vector2i offset;
};

class MouseWheelEvent : public Event
{
public:
	virtual std::string toString() const override;

	bool up;
};

class ControllerButtonEvent : public Event
{
public:
	virtual std::string toString() const override;

	int controller;
	int button;
	bool pressed;
};

class ControllerAxisEvent : public Event
{
public:
	virtual std::string toString() const override;

	int controller;
	int axis;
	float value; // From -1 to +1
};

class ControllerBallEvent : public Event
{
public:
	virtual std::string toString() const override;

	int controller;
	int ball;
	Vector2i offset;
};

