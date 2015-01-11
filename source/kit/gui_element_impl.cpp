#include "gui_element_impl.h"

bool GuiElementImpl::canHaveFocus()
{
	return false;
}

bool GuiElementImpl::hasFocus() const
{
	return false;
}

void GuiElementImpl::setFocus(bool value)
{
}

bool GuiElementImpl::handleEvent(Event const & event, Vector2i cursorPosition, bool cursorPositionIsValid)
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

bool GuiElementImpl::cursorIsOver(Vector2i cursorPosition, bool cursorPositionIsValid) const
{
	return cursorPositionIsValid && getBounds().contains(cursorPosition);
}

