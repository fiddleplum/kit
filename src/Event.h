#pragma once

#include "Vector.h"
#include <string>

class Event
{
public:
	enum Type
	{
		Shutdown, // no class
		Update,
		Keyboard,
		Text,
		MouseButton,
		MouseMove,
		MouseWheel,
		ControllerButton,
		ControllerAxis,
		ControllerBall,
	};

	Event(Type type);

	virtual std::string toString() const = 0;

	template <typename EventType>
	EventType as() const;

	Type type;
};

class ShutdownEvent : public Event
{
public:
	ShutdownEvent();

	virtual std::string toString() const override;
};

class UpdateEvent : public Event
{
public:
	UpdateEvent();

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

	KeyboardEvent();

	virtual std::string toString() const override;

	Key key;
	bool pressed;
};

class TextEvent : public Event
{
public:
	TextEvent();

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

	MouseButtonEvent();

	virtual std::string toString() const override;

	int button;
	bool pressed;
};

class MouseMoveEvent : public Event
{
public:
	MouseMoveEvent();

	virtual std::string toString() const override;

	Vector2i relative;
	Vector2i absolute;
};

class MouseWheelEvent : public Event
{
public:
	MouseWheelEvent();

	virtual std::string toString() const override;

	bool up;
};

class ControllerButtonEvent : public Event
{
public:
	ControllerButtonEvent();

	virtual std::string toString() const override;

	int controller;
	int button;
	bool pressed;
};

class ControllerAxisEvent : public Event
{
public:
	ControllerAxisEvent();

	virtual std::string toString() const override;

	int controller;
	int axis;
	float value; // From -1 to +1
};

class ControllerBallEvent : public Event
{
public:
	ControllerBallEvent();

	virtual std::string toString() const override;

	int controller;
	int ball;
	Vector2i offset;
};

template <typename EventType>
EventType Event::as() const
{
	 return dynamic_cast<EventType const &>(*this);
}

