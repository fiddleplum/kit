#include "gui_container.h"
#include <algorithm>

bool GuiContainer::handleEvent(Event const & event, Ptr<Cursor> cursor)
{
	// Every update, remove the marked elements.
	if(event.type == Event::Update)
	{
		for(Ptr<GuiElement> element : elementsToRemove)
		{
			auto lookupIterator = elementLookup.find(element);
			if(lookupIterator != elementLookup.end())
			{
				elementInfos.erase(lookupIterator->second);
				elementLookup.erase(lookupIterator);
			}
		}
		elementsToRemove.clear();
	}
	bool consumed = handleContainerEvent(event);
	if(consumed)
	{
		return true;
	}
	for(auto elementInfoIterator = elementInfos.rbegin(); elementInfoIterator != elementInfos.rend(); elementInfoIterator++)
	{
		if(elementInfoIterator->active)
		{
			consumed = elementInfoIterator->element->handleEvent(event, cursor);
			if(consumed)
			{
				break;
			}
		}
	}
	return consumed;
}

void GuiContainer::render(Vector2i windowSize) const
{
	for(auto const & elementInfo : elementInfos)
	{
		if(elementInfo.active)
		{
			elementInfo.element->render(windowSize);
		}
	}
}

bool GuiContainer::isElementActive(Ptr<GuiElement> const & element) const
{
	auto lookupIterator = elementLookup.find(element);
	if(lookupIterator == elementLookup.end())
	{
		throw std::runtime_error("GuiContainer::isElementActive, element not found");
	}
	return lookupIterator->second->active;
}

void GuiContainer::setElementActive(Ptr<GuiElement> const & element, bool active)
{
	auto lookupIterator = elementLookup.find(element);
	if(lookupIterator == elementLookup.end())
	{
		throw std::runtime_error("GuiContainer::setElementActive, element not found");
	}
	lookupIterator->second->active = active;
}

void GuiContainer::addElement(OwnPtr<GuiElement> const & element)
{
	auto elementIterator = elementInfos.insert(elementInfos.end(), ElementInfo(element));
	elementLookup[element] = elementIterator;
}

void GuiContainer::removeElement(Ptr<GuiElement> const & element)
{
	elementsToRemove.insert(element);
}

GuiContainer::ElementInfo::ElementInfo(OwnPtr<GuiElement> newElement)
{
	element = newElement;
	active = true;
}

