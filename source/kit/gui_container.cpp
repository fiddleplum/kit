#include "gui_container.h"
#include "open_gl.h"

Recti GuiContainer::getBounds() const
{
	return bounds;
}

void GuiContainer::setPosition(Coord2i position)
{
	bounds.max += position - bounds.min;
	bounds.min = position;
	for(auto const & info : infos)
	{
		updateElementBounds(info);
	}
}

void GuiContainer::setSize(Coord2i size)
{
	bounds.max = bounds.min + size - Coord2i{1, 1};
	for(auto const & info : infos)
	{
		updateElementBounds(info);
	}
}

void GuiContainer::removeElement(Ptr<GuiElement> const & element)
{
	infos.erase(find(element));
	lookup.erase(element);
}

void GuiContainer::moveElementToFront(Ptr<GuiElement> const & element)
{
	auto itOld = find(element);
	ElementInfo info = *itOld; // make a copy so it doesn't destruct
	infos.erase(itOld);
	lookup.erase(element);
	auto itNew = infos.insert(infos.end(), info);
	lookup[info.element] = itNew;
}

void GuiContainer::setElementActive(Ptr<GuiElement> const & element, bool active)
{
	auto it = find(element);
	it->active = active;
}

void GuiContainer::setElementPosition(Ptr<GuiElement> const & element, Coord2f fractionOfElement, Coord2f fractionOfContainer, Coord2i offset)
{
	auto it = find(element);
	it->positionFractionOfElement = fractionOfElement;
	it->positionFractionOfContainer = fractionOfContainer;
	it->positionOffset = offset;
}

void GuiContainer::setElementSize(Ptr<GuiElement> const & element, Coord2f fractionOfContainer, Coord2i offset)
{
	auto it = find(element);
	it->sizeFractionOfContainer = fractionOfContainer;
	it->sizeOffset = offset;
}

void GuiContainer::setContainerEventHandler(std::function<bool(Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid)> handler)
{
	eventHandler = handler;
}

void GuiContainer::setUpdateHandler(std::function<void (float dt)> handler)
{
	updateHandler = handler;
}

void GuiContainer::setPreRenderUpdateHandler(std::function<void ()> handler)
{
	preRenderUpdateHandler = handler;
}

bool GuiContainer::handleEvent(Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid)
{
	bool consumed = false;
	if(eventHandler)
	{
		consumed = eventHandler(event, cursorPosition, cursorPositionIsValid);
	}
	bool cursorPositionIsValidInContainer = cursorPositionIsValid && bounds.contains(cursorPosition);
	for(auto it = infos.rbegin(); !consumed && it != infos.rend(); it++)
	{
		ElementInfo & info = *it;
		if(info.active)
		{
			consumed = info.element->handleEvent(event, cursorPosition, cursorPositionIsValidInContainer);
		}
	}
	return consumed;
}

void GuiContainer::update(float dt)
{
	if(updateHandler)
	{
		updateHandler(dt);
	}
	for(auto const & info : infos)
	{
		if(info.active)
		{
			info.element->update(dt);
		}
	}
}

void GuiContainer::preRenderUpdate()
{
	if(preRenderUpdateHandler)
	{
		preRenderUpdateHandler();
	}
	for(auto const & info : infos)
	{
		if(info.active)
		{
			info.element->preRenderUpdate();
		}
	}
}

void GuiContainer::render(Coord2i windowSize) const
{
	Recti bounds = getBounds();
	glScissorPush(bounds.min[0], bounds.min[1], bounds.max[0] - bounds.min[0] + 1, bounds.max[1] - bounds.min[1] + 1);
	for(auto const & info : infos)
	{
		if(info.active)
		{
			info.element->render(windowSize);
		}
	}
	glScissorPop();
}

std::list<GuiContainer::ElementInfo>::iterator GuiContainer::find(Ptr<GuiElement> const & element) const
{
	auto it = lookup.find(element);
	if(it != lookup.end())
	{
		return it->second;
	}
	else
	{
		throw std::exception();
	}
}

void GuiContainer::updateElementBounds(ElementInfo const & info)
{
	Recti containerBounds = getBounds();
	Coord2i containerSize = containerBounds.max - containerBounds.min + Coord2i{1, 1};
	Recti elementBounds = info.element->getBounds();
	Coord2i elementSize = elementBounds.max - elementBounds.min + Coord2i{1, 1};
	info.element->setSize(Coord2i(info.sizeFractionOfContainer.scale(containerSize)) + info.sizeOffset);
	info.element->setPosition(Coord2i(info.positionFractionOfContainer.scale(containerSize) - info.positionFractionOfElement.scale(elementSize)) + info.positionOffset);
}

