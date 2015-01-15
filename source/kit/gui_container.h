#pragma once

#include "gui_element.h"
#include "ptr.h"
#include <vector>

class GuiContainer
{
public:
	template <typename T> Ptr<T> addElement();

	void removeElement(Ptr<GuiElement> element);

	void moveElementToFront(Ptr<GuiElement> element);

	bool isElementActive(Ptr<GuiElement> element) const;

	void setElementActive(Ptr<GuiElement> element, bool active);

	bool handleEventForElements(Recti bounds, Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid);

	void renderElements(Recti bounds, Coord2i windowSize) const;

private:
	struct ElementInfo
	{
		OwnPtr<GuiElement> element;
		bool active;
	};

	std::vector<ElementInfo> elementInfos;
	bool breakLoop;
};

template <typename T> Ptr<T> GuiContainer::addElement()
{
	auto element = OwnPtr<T>::createNew();
	elementInfos.push_back({element, true});
	breakLoop = true;
	return element;
}

