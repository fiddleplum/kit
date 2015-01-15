#include "gui_layering.h"

void GuiLayering::removeLayer(Ptr<GuiElement> layer)
{
	layers.remove(layer);
}

Recti GuiLayering::getBounds() const
{
	return bounds;
}

void GuiLayering::setPosition(Coord2i position)
{
	bounds.max += position - bounds.min;
	bounds.min = position;
	for(auto layer : layers)
	{
		layer->setPosition(position);
	}
}

void GuiLayering::setSize(Coord2i size)
{
	bounds.max = bounds.min + size - Coord2i{1, 1};
	for(auto layer : layers)
	{
		layer->setSize(size);
	}
}

bool GuiLayering::handleEvent(Event const & event, Coord2i cursorPosition, bool cursorPositionIsValid)
{
	bool cursorPositionIsValidInBounds = cursorPositionIsValid && bounds.contains(cursorPosition);
	for(auto it = layers.rbegin(); it != layers.rend(); ++it)
	{
		auto layer = *it;
		bool consumed = layer->handleEvent(event, cursorPosition, cursorPositionIsValidInBounds);
		if(consumed)
		{
			break;
		}
	}
	return false;
}

void GuiLayering::render(Coord2i windowSize) const
{
	for(auto const & layer : layers)
	{
		layer->render(windowSize);
	}
}

