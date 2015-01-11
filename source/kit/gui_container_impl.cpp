#include "gui_container_impl.h"
#include "gui_sprite_impl.h"
#include "open_gl.h"
#include <algorithm>

void GuiContainerImpl::setSize(Vector2i size)
{
	bounds.max = bounds.min + size - Vector2i{1, 1};
}

void GuiContainerImpl::setEventHandler(std::function<bool(Event const & event, Vector2i cursorPosition, bool cursorPositionIsValid)> handler)
{
	eventHandler = handler;
}

Ptr<GuiSprite> GuiContainerImpl::addSprite()
{
	auto element = OwnPtr<GuiSpriteImpl>::createNew();
	elementInfos.push_back({element, true});
	breakLoop = true;
	return element;
}

void GuiContainerImpl::removeElement(Ptr<GuiElement> element)
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

void GuiContainerImpl::moveElementToFront(Ptr<GuiElement> element)
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

bool GuiContainerImpl::isElementActive(Ptr<GuiElement> element) const
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

void GuiContainerImpl::setElementActive(Ptr<GuiElement> element, bool active)
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

bool GuiContainerImpl::handleEvent(Event const & event, Vector2i cursorPosition, bool cursorPositionIsValid)
{
	bool consumed = false;
	if(eventHandler)
	{
		consumed = eventHandler(event, cursorPosition, cursorPositionIsValid);
		if(consumed)
		{
			return true;
		}
	}
	bool cursorPositionIsValidInContainer = cursorIsOver(cursorPosition, cursorPositionIsValid);
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
	return consumed;
}

void GuiContainerImpl::render(Vector2i windowSize) const
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

