#include <kit/gui/widget_container.h>
#include <algorithm>

namespace kit
{
	namespace gui
	{
		WidgetContainer::WidgetContainer()
		{
			bounds.min = bounds.max = Vector2i::zero();
		}

		Recti WidgetContainer::getBounds() const
		{
			return bounds;
		}

		void WidgetContainer::setHandleContainerEventFunction (std::function<bool (Event const &, bool)> handleContainerEventFunction)
		{
			this->handleContainerEventFunction = handleContainerEventFunction;
		}

		void WidgetContainer::setUpdateWidgetBoundsFunction (std::function<void ()> updateWidgetBoundsFunction)
		{
			this->updateWidgetBoundsFunction = updateWidgetBoundsFunction;
		}

		void WidgetContainer::setPosition(Vector2i position)
		{
			bounds.setMinKeepSize(position);
			if(updateWidgetBoundsFunction)
			{
				updateWidgetBoundsFunction();
			}
		}

		void WidgetContainer::setMaxSize(Vector2i maxSize)
		{
			bounds.setSize(maxSize);
			if(updateWidgetBoundsFunction)
			{
				updateWidgetBoundsFunction();
			}
		}

		void WidgetContainer::removeWidget(Ptr<Widget> widget)
		{
			if(widget.isValid())
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

		void WidgetContainer::setWidgetPlacement(Ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset)
		{
			Vector2i position = bounds.min;
			position = Vector2i(externalFractionalOffset.scale(bounds.getSize()))
						- Vector2i(internalFractionalOffset.scale(widget->getBounds().getSize()))
						+ pixelOffset;
			widget->setPosition(position);
		}

		void WidgetContainer::setWidgetPlacementSize(Ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize)
		{
			Vector2i position = widget->getBounds().min;
			Vector2i maxSize = Vector2i(fractionalSize.scale(bounds.getSize()))
						+ pixelSize;
			maxSize[0] = std::min(maxSize[0], bounds.getSize()[0] - position[0]);
			maxSize[1] = std::min(maxSize[1], bounds.getSize()[1] - position[1]);
			widget->setMaxSize(maxSize);
		}

		bool WidgetContainer::isWidgetActive(Ptr<Widget> widget) const
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error("WidgetContainer::setWidgetVisible, widget not found");
			}
			return lookupIterator->second->active;
		}

		void WidgetContainer::setWidgetActive(Ptr<Widget> widget, bool active)
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error("WidgetContainer::setWidgetActive, widget not found");
			}
			lookupIterator->second->active = active;
		}

		bool WidgetContainer::isWidgetVisible(Ptr<Widget> widget) const
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error("WidgetContainer::setWidgetVisible, widget not found");
			}
			return lookupIterator->second->visible;
		}

		void WidgetContainer::setWidgetVisible(Ptr<Widget> widget, bool visible)
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error("WidgetContainer::setWidgetVisible, widget not found");
			}
			lookupIterator->second->visible = visible;
		}

		WidgetContainer::WidgetInfo::WidgetInfo(OwnPtr<Widget> newWidget)
		{
			active = true;
			visible = true;
			widget = newWidget;
		}

		void WidgetContainer::insertWidgetBefore(OwnPtr<Widget> widget, Ptr<Widget> beforeWidget)
		{
			auto lookupIterator = widgetLookup.find(beforeWidget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error("WidgetContainer::insertWidgetBefore, beforeWidget not found");
			}
			if(widget.isValid())
			{
				auto widgetIterator = widgetInfos.insert(lookupIterator->second, WidgetInfo(widget));
				widgetLookup[widget] = widgetIterator;
			}
		}

		void WidgetContainer::addWidget(OwnPtr<Widget> widget)
		{
			if(widget.isValid())
			{
				auto widgetIterator = widgetInfos.insert(widgetInfos.end(), WidgetInfo(widget));
				widgetLookup[widget] = widgetIterator;
			}
		}

		void WidgetContainer::replaceWidget(OwnPtr<Widget> widget, Ptr<Widget> oldWidget)
		{
			auto lookupIterator = widgetLookup.find(oldWidget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error("WidgetContainer::replaceWidget, widget not found");
			}
			if(widget.isValid())
			{
				lookupIterator->second->widget = widget;
				widgetLookup[widget] = lookupIterator->second;
			}
			else
			{
				widgetInfos.erase(lookupIterator->second);
			}
			widgetLookup.erase(lookupIterator);
		}

		bool WidgetContainer::handleEvent(Event const & event, bool cursorIsValid)
		{
			if(handleContainerEventFunction)
			{
				cursorIsValid = handleContainerEventFunction(event, cursorIsValid) && cursorIsValid;
			}
			for(auto widgetInfoIterator = widgetInfos.rbegin(); widgetInfoIterator != widgetInfos.rend(); widgetInfoIterator++)
			{
				if(widgetInfoIterator->active)
				{
					cursorIsValid = widgetInfoIterator->widget->handleEvent(event, cursorIsValid) && cursorIsValid;
				}
			}
			return cursorIsValid;
		}

		void WidgetContainer::render(Vector2i windowSize)
		{
			for(auto const & widgetInfo : widgetInfos)
			{
				if(widgetInfo.active && widgetInfo.visible)
				{
					widgetInfo.widget->render(windowSize);
				}
			}
		}
	}
}

