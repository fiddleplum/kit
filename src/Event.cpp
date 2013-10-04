#include "Event.h"

Event::Event(Event::Type type)
{
	this->type = type;
}

ShutdownEvent::ShutdownEvent()
	: Event(Shutdown)
{
}

std::string ShutdownEvent::toString() const
{
	return "Shutdown";
}

UpdateEvent::UpdateEvent()
	: Event(Update)
{
}

std::string UpdateEvent::toString() const
{
	return "Update";
}

KeyboardEvent::KeyboardEvent()
	: Event(Keyboard)
{
}

std::string KeyboardEvent::toString() const
{
	std::string r = "Keyboard ";
	switch(key)
	{
		case A: r += "A"; break;
		case B: r += "B"; break;
		case C: r += "C"; break;
		case D: r += "D"; break;
		case E: r += "E"; break;
		case F: r += "F"; break;
		case G: r += "G"; break;
		case H: r += "H"; break;
		case I: r += "I"; break;
		case J: r += "J"; break;
		case K: r += "K"; break;
		case L: r += "L"; break;
		case M: r += "M"; break;
		case N: r += "N"; break;
		case O: r += "O"; break;
		case P: r += "P"; break;
		case Q: r += "Q"; break;
		case R: r += "R"; break;
		case S: r += "S"; break;
		case T: r += "T"; break;
		case U: r += "U"; break;
		case V: r += "V"; break;
		case W: r += "W"; break;
		case X: r += "X"; break;
		case Y: r += "Y"; break;
		case Z: r += "Z"; break;
		case N0: r += "0"; break;
		case N1: r += "1"; break;
		case N2: r += "2"; break;
		case N3: r += "3"; break;
		case N4: r += "4"; break;
		case N5: r += "5"; break;
		case N6: r += "6"; break;
		case N7: r += "7"; break;
		case N8: r += "8"; break;
		case N9: r += "9"; break;
		case Up: r += "Up"; break;
		case Down: r += "Down"; break;
		case Left: r += "Left"; break;
		case Right: r += "Right"; break;
		case PageUp: r += "Page Up"; break;
		case PageDown: r += "Page Down"; break;
		case Home: r += "Home"; break;
		case End: r += "End"; break;
		case Enter: r += "Enter"; break;
		case Backspace: r += "Backspace"; break;
		case Insert: r += "Insert"; break;
		case Delete: r += "Delete"; break;
		case Tab: r += "Tab"; break;
		case Space: r += "Space"; break;
		case Escape: r += "Escape"; break;
		case Keypad0: r += "Keypad 0"; break;
		case Keypad1: r += "Keypad 1"; break;
		case Keypad2: r += "Keypad 2"; break;
		case Keypad3: r += "Keypad 3"; break;
		case Keypad4: r += "Keypad 4"; break;
		case Keypad5: r += "Keypad 5"; break;
		case Keypad6: r += "Keypad 6"; break;
		case Keypad7: r += "Keypad 7"; break;
		case Keypad8: r += "Keypad 8"; break;
		case Keypad9: r += "Keypad 9"; break;
		case KeypadAdd: r += "Keypad Plus"; break;
		case KeypadSubtract: r += "Keypad Minus"; break;
		case KeypadMultiply: r += "Keypad Multiply"; break;
		case KeypadDivide: r += "Keypad Divide"; break;
		case KeypadDecimal: r += "Keypad Decimal"; break;
		case KeypadEnter: r += "Keypad Enter"; break;
		case Grave: r += "Grave"; break;
		case Apostrophe: r += "Apostrophe"; break;
		case Semicolon: r += "Semicolon"; break;
		case Comma: r += "Comma"; break;
		case Period: r += "Period"; break;
		case Slash: r += "Slash"; break;
		case LeftBracket: r += "Left Bracket"; break;
		case RightBracket: r += "Right Bracket"; break;
		case Backslash: r += "Backslash"; break;
		case Hyphen: r += "Hyphen"; break;
		case Equals: r += "Equals"; break;
		case Capslock: r += "Capslock"; break;
		case LeftShift: r += "Left Shift"; break;
		case RightShift: r += "Right Shift"; break;
		case LeftControl: r += "Left Control"; break;
		case RightControl: r += "Right Control"; break;
		case LeftAlt: r += "Left Alt"; break;
		case RightAlt: r += "Right Alt"; break;
		case F1: r += "F1"; break;
		case F2: r += "F2"; break;
		case F3: r += "F3"; break;
		case F4: r += "F4"; break;
		case F5: r += "F5"; break;
		case F6: r += "F6"; break;
		case F7: r += "F7"; break;
		case F8: r += "F8"; break;
		case F9: r += "F9"; break;
		case F10: r += "F10"; break;
		case F11: r += "F11"; break;
		case F12: r += "F12"; break;
		case F13: r += "F13"; break;
		case F14: r += "F14"; break;
		case F15: r += "F15"; break;
		case Pause: r += "Pause"; break;
	}
	r += std::string(": ") + (pressed ? "Pressed" : "Unpressed");
	return r;
}

TextEvent::TextEvent()
	: Event(Text)
{
}

std::string TextEvent::toString() const
{
	return "Text: " + text;
}

MouseButtonEvent::MouseButtonEvent()
	: Event(MouseButton)
{
}

std::string MouseButtonEvent::toString() const
{
	std::string r = "Mouse Button ";
	switch(button)
	{
		case Left: r += "Left"; break;
		case Middle: r += "Middle"; break;
		case Right: r += "Right"; break;
		default: r += std::to_string(button); break;
	}
	r += std::string(": ") + (pressed ? "Pressed" : "Unpressed");
	return r;
}

MouseMoveEvent::MouseMoveEvent()
	: Event(MouseMove)
{
}

std::string MouseMoveEvent::toString() const
{
	return "Mouse Move: " + std::to_string(relative[0]) + ", " + std::to_string(relative[1]);
}

MouseWheelEvent::MouseWheelEvent()
	: Event(MouseWheel)
{
}

std::string MouseWheelEvent::toString() const
{
	return std::string("Mouse Wheel: ") + (up ? "Up" : "Down");
}

ControllerButtonEvent::ControllerButtonEvent()
	: Event(ControllerButton)
{
}

std::string ControllerButtonEvent::toString() const
{
	return "Controller " + std::to_string(controller) + " Button " + std::to_string(button) + ": " + (pressed ? "Pressed" : "Unpressed");
}

ControllerAxisEvent::ControllerAxisEvent()
	: Event(ControllerAxis)
{
}

std::string ControllerAxisEvent::toString() const
{
	return "Controller " + std::to_string(controller) + " Axis " + std::to_string(axis) + ": " + std::to_string(value);
}

ControllerBallEvent::ControllerBallEvent()
	: Event(ControllerBall)
{
}

std::string ControllerBallEvent::toString() const
{
	return "Controller " + std::to_string(controller) + " Ball " + std::to_string(ball) + ": " + std::to_string(offset[0]) + ", " + std::to_string(offset[1]);
}

