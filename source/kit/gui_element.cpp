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

bool GuiElement::cursorIsOver(Coord2i cursorPosition, bool cursorPositionIsValid) const
{
	return cursorPositionIsValid && getBounds().contains(cursorPosition);
}

