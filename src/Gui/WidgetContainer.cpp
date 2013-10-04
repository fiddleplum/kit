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
		for(WidgetPlacement & widgetPlacement : widgetPlacements)
		{
			setWidgetBounds(widgetPlacement);
		}
	}

	void WidgetContainer::setMaxSize(Vector2i maxSize)
	{
		bounds.setSize(maxSize);
		for(WidgetPlacement & widgetPlacement : widgetPlacements)
		{
			setWidgetBounds(widgetPlacement);
		}
	}

	void WidgetContainer::handleEvent(Event const & event)
	{
		for(WidgetPlacement & widgetPlacement : widgetPlacements)
		{
			widgetPlacement.widget->handleEvent(event);
		}
	}

	void WidgetContainer::render()
	{
		for(WidgetPlacement & widgetPlacement : widgetPlacements)
		{
			widgetPlacement.widget->render();
		}
	}

	void WidgetContainer::addWidget(std::shared_ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i offset)
	{
		if(widget == nullptr)
		{
			throw std::runtime_error("WidgetContainer::addWidget, widget == nullptr");
		}
		WidgetPlacement wp;
		wp.widget = widget;
		wp.externalFractionalOffset = externalFractionalOffset;
		wp.internalFractionalOffset = internalFractionalOffset;
		wp.offset = offset;
		wp.fractionalSizeOffset = Vector2f::one();
		wp.sizeOffset = Vector2i::zero();
		widgetPlacements.push_back(wp);
		setWidgetBounds(widgetPlacements.back());
	}

	void WidgetContainer::setWidgetPlacementSize(std::shared_ptr<Widget> widget, Vector2f fractionalOffset, Vector2i offset)
	{
		for(auto it = widgetPlacements.begin(); it != widgetPlacements.end(); it++)
		{
			if(it->widget == widget)
			{
				it->fractionalSizeOffset = fractionalOffset;
				it->sizeOffset = offset;
				setWidgetBounds(*it);
				break;
			}
		}
	}

	void WidgetContainer::removeWidget(std::shared_ptr<Widget> widget)
	{
		for(auto it = widgetPlacements.begin(); it != widgetPlacements.end(); it++)
		{
			if(it->widget == widget)
			{
				widgetPlacements.erase(it);
				break;
			}
		}
	}

	void WidgetContainer::setWidgetBounds(WidgetPlacement & widgetPlacement)
	{
		widgetPlacement.widget->setMaxSize(bounds.getSize()); // first set it to the container size so it can positioned correctly
		Vector2i position = bounds.min;
		position = Vector2i(widgetPlacement.externalFractionalOffset.scale(bounds.getSize()))
					- Vector2i(widgetPlacement.internalFractionalOffset.scale(widgetPlacement.widget->getBounds().getSize()))
					+ widgetPlacement.offset;
		widgetPlacement.widget->setPosition(position);
		Vector2i maxSize = Vector2i(widgetPlacement.fractionalSizeOffset.scale(bounds.getSize()))
					+ widgetPlacement.offset;
		maxSize[0] = std::min(maxSize[0], bounds.getSize()[0] - position[0]);
		maxSize[1] = std::min(maxSize[1], bounds.getSize()[1] - position[1]);
		widgetPlacement.widget->setMaxSize(maxSize);
	}
}

