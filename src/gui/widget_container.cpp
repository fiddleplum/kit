#include "widget_container_p.h"
#include <algorithm>
#include <list>
#include <map>

namespace kit
{
	namespace gui
	{
		void WidgetContainerP::setHandleContainerEventFunction (std::function<void (Event const &)> newHandleContainerEventFunction)
		{
			handleContainerEventFunction = newHandleContainerEventFunction;
		}

		void WidgetContainerP::setUpdateWidgetBoundsFunction (std::function<void ()> newUpdateWidgetBoundsFunction)
		{
			updateWidgetBoundsFunction = newUpdateWidgetBoundsFunction;
		}

		void WidgetContainerP::callUpdateWidgetBoundsFunction ()
		{
			if(updateWidgetBoundsFunction)
			{
				updateWidgetBoundsFunction();
			}
		}

		Recti WidgetContainerP::getBounds () const
		{
			return bounds;
		}

		void WidgetContainerP::setPosition (Vector2i position)
		{
			bounds.setMinKeepSize(position);
			if(updateWidgetBoundsFunction)
			{
				updateWidgetBoundsFunction();
			}
		}

		void WidgetContainerP::setMaxSize (Vector2i maxSize)
		{
			bounds.setSize(maxSize);
			if(updateWidgetBoundsFunction)
			{
				updateWidgetBoundsFunction();
			}
		}

		void WidgetContainerP::insertWidgetBefore (UsePtr<Widget> widget, UsePtr<Widget> beforeWidget)
		{
			auto lookupIterator = widgetLookup.find(beforeWidget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainerP::insertWidgetBefore, beforeWidget not found");
			}
			auto widgetIterator = widgetInfos.insert(lookupIterator->second, WidgetInfo(widget));
			widgetLookup[widget] = widgetIterator;
		}

		void WidgetContainerP::addWidget (UsePtr<Widget> widget)
		{
			auto widgetIterator = widgetInfos.insert(widgetInfos.end(), WidgetInfo(widget));
			widgetLookup[widget] = widgetIterator;
			widget->setPosition(bounds.min);
			widget->setMaxSize(bounds.getSize());
		}

		void WidgetContainerP::removeWidget (UsePtr<Widget> widget)
		{
			if(widget.isValid())
			{
				auto lookupIterator = widgetLookup.find(widget);
				if(lookupIterator == widgetLookup.end())
				{
					throw std::runtime_error ("WidgetContainerP::removeWidget, widget not found");
				}
				widgetInfos.erase(lookupIterator->second);
				widgetLookup.erase(lookupIterator);
			}
		}

		void WidgetContainerP::setWidgetPlacement (UsePtr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset)
		{
			Vector2i position = bounds.min;
			position = Vector2i(externalFractionalOffset.scale(bounds.getSize())) - Vector2i(internalFractionalOffset.scale(widget->getBounds().getSize())) + pixelOffset;
			widget->setPosition(position);
		}

		void WidgetContainerP::setWidgetPlacementSize (UsePtr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize)
		{
			Vector2i position = widget->getBounds().min;
			Vector2i maxSize = Vector2i(fractionalSize.scale(bounds.getSize())) + pixelSize;
			maxSize[0] = std::min(maxSize[0], bounds.getSize()[0] - position[0]);
			maxSize[1] = std::min(maxSize[1], bounds.getSize()[1] - position[1]);
			widget->setMaxSize(maxSize);
		}

		bool WidgetContainerP::isWidgetActive (UsePtr<Widget> widget) const
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainerP::setWidgetVisible, widget not found");
			}
			return lookupIterator->second->active;
		}

		void WidgetContainerP::setWidgetActive (UsePtr<Widget> widget, bool active)
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainerP::setWidgetActive, widget not found");
			}
			lookupIterator->second->active = active;
		}

		bool WidgetContainerP::isWidgetVisible (UsePtr<Widget> widget) const
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainerP::setWidgetVisible, widget not found");
			}
			return lookupIterator->second->visible;
		}

		void WidgetContainerP::setWidgetVisible (UsePtr<Widget> widget, bool visible)
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainerP::setWidgetVisible, widget not found");
			}
			lookupIterator->second->visible = visible;
		}

		void WidgetContainerP::handleEvent (Event const & event)
		{
			if(handleContainerEventFunction)
			{
				handleContainerEventFunction(event);
			}
			for(auto widgetInfoIterator = widgetInfos.rbegin(); widgetInfoIterator != widgetInfos.rend(); widgetInfoIterator++)
			{
				if(widgetInfoIterator->active)
				{
					widgetInfoIterator->widget->handleEvent(event);
				}
			}
		}

		void WidgetContainerP::render (Vector2i windowSize)
		{
			for(auto const & widgetInfo : widgetInfos)
			{
				if(widgetInfo.active && widgetInfo.visible)
				{
					widgetInfo.widget->render(windowSize);
				}
			}
		}

		WidgetContainerP::WidgetInfo::WidgetInfo (UsePtr<Widget> newWidget)
		{
			widget = newWidget.as<WidgetP>();
			active = true;
			visible = true;
		}
	}
}

