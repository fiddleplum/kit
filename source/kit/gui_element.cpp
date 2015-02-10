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

bool GuiElement::handleEvent(Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid)
{
	if(event.is(Event::MouseButton))
	{
		auto mbEvent = event.as<MouseButtonEvent>();
		if(mbEvent.pressed && cursorIsOver(cursorPosition, cursorPositionIsValid))
		{
			return true;
		}
	}
	else if(event.is(Event::MouseWheel))
	{
		if(cursorIsOver(cursorPosition, cursorPositionIsValid))
		{
			return true;
		}
	}
	return false;
}

bool GuiElement::cursorIsOver(Coord2i cursorPosition, bool cursorPositionIsValid) const
{
	return cursorPositionIsValid && getBounds().contains(cursorPosition);
}

