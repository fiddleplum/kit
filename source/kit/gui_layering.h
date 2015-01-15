#pragma once

#include "gui_element.h"
#include "object_vector.h"
#include "ptr.h"

class GuiLayering : public GuiElement
{
public:
	// Adds a layer. T must be a subclass of GuiElement.
	template <typename T> Ptr<T> addLayer();

	// Removes a layer.
	void removeLayer(Ptr<GuiElement> layer);

	Recti getBounds() const override;

	void setPosition(Coord2i position) override;

	void setSize(Coord2i size) override;

	bool handleEvent(Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid) override;

	void render(Coord2i windowSize) const override;

private:
	Recti bounds;
	ObjectVector<OwnPtr<GuiElement>> layers;
};

template <typename T>
Ptr<T> GuiLayering::addLayer()
{
	OwnPtr<T> element = OwnPtr<T>::createNew();
	layers.add(element);
	return element;
}

