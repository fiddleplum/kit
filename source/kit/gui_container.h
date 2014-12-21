#pragma once

#include "gui_element.h"
#include "ptr.h"
#include <list>
#include <set>
#include <map>

class GuiContainer : public GuiElement
{
public:
	// Implements GuiElement::handleEvent.
	bool handleEvent(Event const & event, Ptr<Cursor> cursor) override;

	// Implements GuiElement::render.
	void render(Vector2i windowSize) const override;

	// Does the widget handle events and rendered?
	bool isElementActive(Ptr<GuiElement> const & widget) const;

	// Sets whether the widget handles events and rendered.
	void setElementActive(Ptr<GuiElement> const & widget, bool active);

protected:
	// Handles container-wide events. Returns true if the event is consumed.
	virtual bool handleContainerEvent(Event const & event) = 0;

	// Add a widget. It will appear on top of all previous widgets.
	template <typename Type> Ptr<Type> addElement();

	// Insert the widget before another widget. If beforeGuiElement is null, the widget is inserted at the end.
	template <typename Type> Ptr<Type> insertElement(Ptr<GuiElement> const & beforeElement);

	// Remove a widget.
	void removeElement(Ptr<GuiElement> const & element);

private:
	class ElementInfo
	{
	public:
		ElementInfo(OwnPtr<GuiElement> element);

		OwnPtr<GuiElement> element;
		bool active;
	};

	void addElement(OwnPtr<GuiElement> const & element);
	void insertElement(OwnPtr<GuiElement> const & beforeElement);

	std::list<ElementInfo> elementInfos;
	std::map<Ptr<GuiElement>, std::list<ElementInfo>::iterator> elementLookup;
	std::set<Ptr<GuiElement>> elementsToRemove;
};

template <typename Type> Ptr<Type> GuiContainer::addElement()
{
	OwnPtr<Type> widget;
	widget.create();
	addElement(widget);
	return widget;
}

template <typename Type> Ptr<Type> GuiContainer::insertElement(Ptr<GuiElement> const & beforeElement)
{
	OwnPtr<Type> widget;
	widget.create();
	insertElement(widget, beforeElement);
	return widget;
}

