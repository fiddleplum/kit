#pragma once

#include "vector.h"
#include "ptr.h"
#include <string>

namespace kit
{
	class Window;

	class Event
	{
	public:
		enum Type
		{
			Shutdown,
			Resize,
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

		Event(Ptr<Window> window, Type type);

		virtual std::string toString() const = 0;

		template <typename EventType>
		EventType as() const;

		Ptr<Window> window;
		Type type;
	};

	class ShutdownEvent : public Event
	{
	public:
		ShutdownEvent(Ptr<Window> window);

		virtual std::string toString() const override;
	};

	class ResizeEvent : public Event
	{
	public:
		ResizeEvent(Ptr<Window> window);

		virtual std::string toString() const override;

		Vector2i size;
	};

	class UpdateEvent : public Event
	{
	public:
		UpdateEvent(Ptr<Window> window);

		virtual std::string toString() const override;
	};

	class KeyboardEvent : public Event
	{
	public:
		enum Key
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

		KeyboardEvent(Ptr<Window> window);

		virtual std::string toString() const override;

		Key key;
		bool pressed;
	};

	class TextEvent : public Event
	{
	public:
		TextEvent(Ptr<Window> window);

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

		MouseButtonEvent(Ptr<Window> window);

		virtual std::string toString() const override;

		int button;
		bool pressed;
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(Ptr<Window> window);

		virtual std::string toString() const override;

		Vector2i relative;
		Vector2i absolute;
	};

	class MouseWheelEvent : public Event
	{
	public:
		MouseWheelEvent(Ptr<Window> window);

		virtual std::string toString() const override;

		bool up;
	};

	class ControllerButtonEvent : public Event
	{
	public:
		ControllerButtonEvent(Ptr<Window> window);

		virtual std::string toString() const override;

		int controller;
		int button;
		bool pressed;
	};

	class ControllerAxisEvent : public Event
	{
	public:
		ControllerAxisEvent(Ptr<Window> window);

		virtual std::string toString() const override;

		int controller;
		int axis;
		float value; // From -1 to +1
	};

	class ControllerBallEvent : public Event
	{
	public:
		ControllerBallEvent(Ptr<Window> window);

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
}

/*

Notes on pass-thru

MouseMove should make its cursor invalid if the widget that it is handling it deems so.
Keyboard should stop after the first widget consumes it.
MouseButton 

Should every widget have a contains function? Then in a widget container, if a widget contains the cursor, the MouseMove is automatically
changed to cursorValid = false, and MouseButton left click sets that widget as the focus widget. Then all keyboard commands are sent to the focus widget
first, then to the other widgets (if it isn't consumed already)

*/