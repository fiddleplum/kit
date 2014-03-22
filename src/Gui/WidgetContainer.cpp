#include "WidgetContainer.h"
#include <algorithm>

namespace Gui
{
	WidgetContainer::WidgetContainer()
	{
		bounds.min = bounds.max = Vector2i::zero();
	}

	Recti WidgetContainer::getBounds() const
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

	void WidgetContainer::insertWidgetBefore(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> beforeWidget)
	{
		if(widget != nullptr)
		{
			auto lookupIterator = widgetLookup.find(beforeWidget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error("WidgetContainer::insertWidgetBefore, beforeWidget not found");
			}
			auto widgetIterator = widgetInfos.insert(lookupIterator->second, WidgetInfo(widget));
			widgetLookup[widget] = widgetIterator;
		}
	}

	void WidgetContainer::addWidget(std::shared_ptr<Widget> widget)
	{
		if(widget != nullptr)
		{
			auto widgetIterator = widgetInfos.insert(widgetInfos.end(), WidgetInfo(widget));
			widgetLookup[widget] = widgetIterator;
		}
	}

	void WidgetContainer::removeWidget(std::shared_ptr<Widget> widget)
	{
		if(widget != nullptr)
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error("WidgetContainer::removeWidget, widget not found");
			}
			widgetInfos.erase(lookupIterator->second);
			widgetLookup.erase(lookupIterator);
		}
	}

	void WidgetContainer::replaceWidget(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> newWidget)
	{
		auto lookupIterator = widgetLookup.find(widget);
		if(lookupIterator == widgetLookup.end())
		{
			throw std::runtime_error("WidgetContainer::replaceWidget, widget not found");
		}
		if(newWidget != nullptr)
		{
			lookupIterator->second->widget = newWidget;
			widgetLookup[newWidget] = lookupIterator->second;
		}
		else
		{
			widgetInfos.erase(lookupIterator->second);
		}
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

	bool WidgetContainer::isWidgetVisible(std::shared_ptr<Widget> widget) const
	{
		auto lookupIterator = widgetLookup.find(widget);
		if(lookupIterator == widgetLookup.end())
		{
			throw std::runtime_error("WidgetContainer::setWidgetVisible, widget not found");
		}
		return lookupIterator->second->visible;
	}

	bool WidgetContainer::isWidgetActive(std::shared_ptr<Widget> widget) const
	{
		auto lookupIterator = widgetLookup.find(widget);
		if(lookupIterator == widgetLookup.end())
		{
			throw std::runtime_error("WidgetContainer::setWidgetVisible, widget not found");
		}
		return lookupIterator->second->active;
	}

	void WidgetContainer::setWidgetVisible(std::shared_ptr<Widget> widget, bool visible)
	{
		auto lookupIterator = widgetLookup.find(widget);
		if(lookupIterator == widgetLookup.end())
		{
			throw std::runtime_error("WidgetContainer::setWidgetVisible, widget not found");
		}
		lookupIterator->second->visible = visible;
	}

	void WidgetContainer::setWidgetActive(std::shared_ptr<Widget> widget, bool active)
	{
		auto lookupIterator = widgetLookup.find(widget);
		if(lookupIterator == widgetLookup.end())
		{
			throw std::runtime_error("WidgetContainer::setWidgetActive, widget not found");
		}
		lookupIterator->second->active = active;
	}

	WidgetContainer::WidgetInfo::WidgetInfo(std::shared_ptr<Widget> newWidget)
	{
		active = true;
		visible = true;
		widget = newWidget;
	}

	void WidgetContainer::handleEvent(Event const & event)
	{
		handleContainerEvent(event);
		for(auto widgetInfoIterator = widgetInfos.rbegin(); widgetInfoIterator != widgetInfos.rend(); widgetInfoIterator++)
		{
			if(widgetInfoIterator->active && widgetInfoIterator->visible)
			{
				widgetInfoIterator->widget->handleEvent(event);
			}
		}
	}

	void WidgetContainer::render()
	{
		for(auto const & widgetInfo : widgetInfos)
		{
			if(widgetInfo.visible)
			{
				widgetInfo.widget->render();
			}
		}
	}
}

