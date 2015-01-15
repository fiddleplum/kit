#include "gui_container.h"
#include "open_gl.h"

void GuiContainer::removeElement(Ptr<GuiElement> element)
{
	for(auto it = elementInfos.begin(); it != elementInfos.end(); it++)
	{
		if(it->element == element)
		{
			elementInfos.erase(it);
			breakLoop = true;
			return;
		}
	}
	throw std::exception(); // Element not found.
}

void GuiContainer::moveElementToFront(Ptr<GuiElement> element)
{
	for(auto it = elementInfos.begin(); it != elementInfos.end(); it++)
	{
		if(it->element == element)
		{
			ElementInfo ei = *it;
			elementInfos.erase(it);
			elementInfos.push_back(ei);
			breakLoop = true;
			return;
		}
	}
	throw std::exception(); // Element not found.
}

bool GuiContainer::isElementActive(Ptr<GuiElement> element) const
{
	for(auto it = elementInfos.begin(); it != elementInfos.end(); it++)
	{
		if(it->element == element)
		{
			return it->active;
		}
	}
	throw std::exception(); // Element not found.
}

void GuiContainer::setElementActive(Ptr<GuiElement> element, bool active)
{
	for(auto it = elementInfos.begin(); it != elementInfos.end(); it++)
	{
		if(it->element == element)
		{
			it->active = active;
		}
	}
	throw std::exception(); // Element not found.
}

bool GuiContainer::handleEventForElements(Recti bounds, Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid)
{
	bool consumed = false;
	bool cursorPositionIsValidInContainer = cursorPositionIsValid && bounds.contains(cursorPosition);
	breakLoop = false;
	for(auto it = elementInfos.rbegin(); it != elementInfos.rend(); it++)
	{
		if(it->active)
		{
			consumed = it->element->handleEvent(event, cursorPosition, cursorPositionIsValidInContainer);
			if(consumed || breakLoop)
			{
				break;
			}
		}
	}
	breakLoop = false;
	return consumed;
}

void GuiContainer::renderElements(Recti bounds, Coord2i windowSize) const
{
	glScissorPush(bounds.min[0], bounds.min[1], bounds.max[0] - bounds.min[0] + 1, bounds.max[1] - bounds.min[1] + 1);
	for(auto const & elementInfo : elementInfos)
	{
		if(elementInfo.active)
		{
			elementInfo.element->render(windowSize);
		}
	}
	glScissorPop();
}

