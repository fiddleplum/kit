#include "gui_element.h"

bool GuiElement::canHaveFocus()
{
	return false;
}

bool GuiElement::hasFocus() const
{
	return false;
}

void GuiElement::setFocus(bool value)
{
}

bool GuiElement::handleEvent(Event const & event, Vector2i cursorPosition, bool cursorPositionIsValid)
{
	if(event.type == Event::MouseButton)
	{
		auto mbEvent = event.as<MouseButtonEvent>();
		if(mbEvent.pressed == true && cursorPositionIsValid && getBounds().containsEx(cursorPosition))
		{
			return true;
		}
	}
	else if(event.type == Event::MouseWheel)
	{
		auto mwEvent = event.as<MouseWheelEvent>();
		if(cursorPositionIsValid && getBounds().containsEx(cursorPosition))
		{
			return true;
		}
	}
	return false;
}
