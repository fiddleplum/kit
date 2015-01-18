#pragma once

#include "gui_element.h"
#include "ptr.h"
#include <map>
#include <list>
#include <functional>

class GuiContainer : public GuiElement
{
public:
	Recti getBounds() const override;

	void setPosition(Coord2i position) override;

	void setSize(Coord2i size) override;

	template <typename T> Ptr<T> addElement();

	void removeElement(Ptr<GuiElement> const & element);

	void moveElementToFront(Ptr<GuiElement> const & element);

	void setElementActive(Ptr<GuiElement> const & element, bool active);

	void setElementPosition(Ptr<GuiElement> const & element, Coord2f fractionOfContainer, Coord2f fractionOfElement, Coord2i offset);

	void setElementSize(Ptr<GuiElement> const & element, Coord2f fractionOfContainer, Coord2i offset);

	void setContainerEventHandler(std::function<bool (Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid)> handler);

	void setUpdateHandler(std::function<void (float dt)> handler);

	void setPreRenderUpdateHandler(std::function<void ()> handler);

	bool handleEvent(Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid) override;

	void update(float dt) override;

	void preRenderUpdate() override;

	void render(Coord2i windowSize) const override;

private:
	class ElementInfo
	{
	public:
		OwnPtr<GuiElement> element;
		bool active;
		Coord2f positionFractionOfElement;
		Coord2f positionFractionOfContainer;
		Coord2i positionOffset;
		Coord2f sizeFractionOfContainer;
		Coord2i sizeOffset;
	};

	std::list<ElementInfo>::iterator find(Ptr<GuiElement> const & element) const;
	void updateElementBounds(ElementInfo const & info);

	Recti bounds;
	std::list<ElementInfo> infos;
	std::map<Ptr<GuiElement>, std::list<ElementInfo>::iterator> lookup;
	std::function<bool (Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid)> eventHandler;
	std::function<void (float dt)> updateHandler;
	std::function<void ()> preRenderUpdateHandler;
};

template <typename T> Ptr<T> GuiContainer::addElement()
{
	ElementInfo info;
	OwnPtr<T> elementDerived = OwnPtr<T>::createNew();
	info.element = elementDerived;
	info.active = true;
	info.sizeFractionOfContainer = {1, 1};
	info.positionFractionOfElement = {0, 0};
	info.positionFractionOfContainer = {0, 0};
	info.positionOffset = {0, 0};
	auto it = infos.insert(infos.end(), info);
	lookup[info.element] = it;
	updateElementBounds(info);
	return elementDerived;
}

