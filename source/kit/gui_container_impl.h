#pragma once

#include "gui_element_impl.h"
#include <kit/gui_container.h>
#include <list>
#include <set>
#include <map>

class GuiContainerImpl : virtual public GuiContainer, virtual public GuiElementImpl
{
public:
	void setSize(Vector2i size) override;

	void setEventHandler(std::function<bool(Event const & event, Vector2i cursorPosition, bool cursorPositionIsValid)> handler) override;

	Ptr<GuiSprite> addSprite() override;

	void removeElement(Ptr<GuiElement> element) override;

	void moveElementToFront(Ptr<GuiElement> element) override;

	bool isElementActive(Ptr<GuiElement> element) const override;

	void setElementActive(Ptr<GuiElement> element, bool active) override;

	bool handleEvent(Event const & event, Vector2i cursorPosition, bool cursorPositionIsValid) override;

	void render(Vector2i windowSize) const override;

private:
	struct ElementInfo
	{
		OwnPtr<GuiElementImpl> element;
		bool active;
	};

	std::list<ElementInfo> elementInfos;
	bool breakLoop;
	Recti bounds;
	std::function<bool (Event const & event, Vector2i cursorPosition, bool cursorPositionIsValid)> eventHandler;
};

