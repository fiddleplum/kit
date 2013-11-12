#include "WidgetContainer.h"
#include <algorithm>

namespace Gui
{
	WidgetContainer::WidgetContainer()
	{
		bounds.min = bounds.max = Vector2i::zero();
	}

	Box2i WidgetContainer::getBounds() const
	{
		return bounds;
	}

	void WidgetContainer::setPosition(Vector2i position)
	{
		bounds.setMinKeepSize(position);
		updateWidgetBounds();
	}

	void WidgetContainer::setMaxSize(Vector2i maxSize)
	{
		bounds.setSize(maxSize);
		updateWidgetBounds();
	}

	void WidgetContainer::handleEvent(Event const & event)
	{
		for(auto widgetIterator = widgets.rbegin(); widgetIterator != widgets.rend(); widgetIterator++)
		{
			(*widgetIterator)->handleEvent(event);
		}
	}

	void WidgetContainer::render()
	{
		for(auto widget : widgets)
		{
			widget->render();
		}
	}

	void WidgetContainer::insertWidgetBefore(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> beforeWidget)
	{
		if(widget == nullptr)
		{
			throw std::runtime_error("WidgetContainer::insertWidgetBefore, widget == nullptr");
		}
		auto lookupIterator = widgetLookup.find(beforeWidget);
		if(lookupIterator == widgetLookup.end())
		{
			throw std::runtime_error("WidgetContainer::insertWidgetBefore, widgetBefore not found");
		}
		auto widgetIterator = widgets.insert(lookupIterator->second, widget);
		widgetLookup[widget] = widgetIterator;
	}

	void WidgetContainer::appendWidget(std::shared_ptr<Widget> widget)
	{
		if(widget == nullptr)
		{
			throw std::runtime_error("WidgetContainer::appendWidget, widget == nullptr");
		}
		auto widgetIterator = widgets.insert(widgets.end(), widget);
		widgetLookup[widget] = widgetIterator;
	}

	void WidgetContainer::removeWidget(std::shared_ptr<Widget> widget)
	{
		auto lookupIterator = widgetLookup.find(widget);
		if(lookupIterator == widgetLookup.end())
		{
			throw std::runtime_error("WidgetContainer::removeWidget, widget not found");
		}
		widgets.erase(lookupIterator->second);
		widgetLookup.erase(lookupIterator);
	}

	void WidgetContainer::setWidgetPlacement(std::shared_ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset)
	{
		Vector2i position = bounds.min;
		position = Vector2i(externalFractionalOffset.scale(bounds.getSize()))
					- Vector2i(internalFractionalOffset.scale(widget->getBounds().getSize()))
					+ pixelOffset;
		widget->setPosition(position);
	}

	void WidgetContainer::setWidgetPlacementSize(std::shared_ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize)
	{
		Vector2i position = widget->getBounds().min;
		Vector2i maxSize = Vector2i(fractionalSize.scale(bounds.getSize()))
					+ pixelSize;
		maxSize[0] = std::min(maxSize[0], bounds.getSize()[0] - position[0]);
		maxSize[1] = std::min(maxSize[1], bounds.getSize()[1] - position[1]);
		widget->setMaxSize(maxSize);
	}
}

