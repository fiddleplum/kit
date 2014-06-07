#include "widget_container_internal.h"
#include <kit/gui/widget_container.h>
#include "sprite_internal.h"
#include <algorithm>

namespace kit
{
	namespace gui
	{
		OwnPtr<WidgetInternal> createWidget (WidgetType type)
		{
			switch(type)
			{
			case SPRITE:
				return OwnPtr<SpriteInternal> (new SpriteInternal);
				break;
			case BUTTON:
				//return OwnPtr<Button> (new Button);
				break;
			}
			return OwnPtr<WidgetInternal> ();
		}

		WidgetContainerInternal::WidgetContainerInternal ()
		{
			bounds.min = bounds.max = Vector2i::zero();
		}

		void WidgetContainerInternal::setHandleContainerEventFunction (std::function<bool (Event const &, bool)> handleContainerEventFunction)
		{
			handleContainerEventFunction = handleContainerEventFunction;
		}

		void WidgetContainerInternal::setUpdateWidgetBoundsFunction (std::function<void ()> updateWidgetBoundsFunction)
		{
			updateWidgetBoundsFunction = updateWidgetBoundsFunction;
		}

		Recti WidgetContainerInternal::getBounds () const
		{
			return bounds;
		}

		void WidgetContainerInternal::setPosition (Vector2i position)
		{
			bounds.setMinKeepSize(position);
			if(updateWidgetBoundsFunction)
			{
				updateWidgetBoundsFunction();
			}
		}

		void WidgetContainerInternal::setMaxSize (Vector2i maxSize)
		{
			bounds.setSize(maxSize);
			if(updateWidgetBoundsFunction)
			{
				updateWidgetBoundsFunction();
			}
		}

		Ptr<Widget> WidgetContainerInternal::insertWidgetBefore (WidgetType type, Ptr<Widget> beforeWidget)
		{
			auto lookupIterator = widgetLookup.find(beforeWidget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainerInternal::insertWidgetBefore, beforeWidget not found");
			}
			OwnPtr<WidgetInternal> widget = createWidget(type);
			if(widget.isValid())
			{
				auto widgetIterator = widgetInfos.insert(lookupIterator->second, WidgetInfo(widget));
				widgetLookup[widget] = widgetIterator;
			}
			return widget;
		}

		Ptr<Widget> WidgetContainerInternal::addWidget (WidgetType type)
		{
			OwnPtr<WidgetInternal> widget = createWidget(type);
			if(widget.isValid())
			{
				auto widgetIterator = widgetInfos.insert(widgetInfos.end(), WidgetInfo(widget));
				widgetLookup[widget] = widgetIterator;
			}
			return widget;
		}

		void WidgetContainerInternal::removeWidget (Ptr<Widget> widget)
		{
			if(widget.isValid())
			{
				auto lookupIterator = widgetLookup.find(widget);
				if(lookupIterator == widgetLookup.end())
				{
					throw std::runtime_error ("WidgetContainerInternal::removeWidget, widget not found");
				}
				widgetInfos.erase(lookupIterator->second);
				widgetLookup.erase(lookupIterator);
			}
		}

		void WidgetContainerInternal::setWidgetPlacement (Ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset)
		{
			Vector2i position = bounds.min;
			position = Vector2i(externalFractionalOffset.scale(bounds.getSize())) - Vector2i(internalFractionalOffset.scale(widget->getBounds().getSize())) + pixelOffset;
			widget->setPosition(position);
		}

		void WidgetContainerInternal::setWidgetPlacementSize (Ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize)
		{
			Vector2i position = widget->getBounds().min;
			Vector2i maxSize = Vector2i(fractionalSize.scale(bounds.getSize())) + pixelSize;
			maxSize[0] = std::min(maxSize[0], bounds.getSize()[0] - position[0]);
			maxSize[1] = std::min(maxSize[1], bounds.getSize()[1] - position[1]);
			widget->setMaxSize(maxSize);
		}

		bool WidgetContainerInternal::isWidgetActive (Ptr<Widget> widget) const
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainerInternal::setWidgetVisible, widget not found");
			}
			return lookupIterator->second->active;
		}

		void WidgetContainerInternal::setWidgetActive (Ptr<Widget> widget, bool active)
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainerInternal::setWidgetActive, widget not found");
			}
			lookupIterator->second->active = active;
		}

		bool WidgetContainerInternal::isWidgetVisible (Ptr<Widget> widget) const
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainerInternal::setWidgetVisible, widget not found");
			}
			return lookupIterator->second->visible;
		}

		void WidgetContainerInternal::setWidgetVisible (Ptr<Widget> widget, bool visible)
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainerInternal::setWidgetVisible, widget not found");
			}
			lookupIterator->second->visible = visible;
		}

		bool WidgetContainerInternal::handleEvent (Event const & event, bool cursorIsValid)
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

		void WidgetContainerInternal::render (Vector2i windowSize)
		{
			for(auto const & widgetInfo : widgetInfos)
			{
				if(widgetInfo.active && widgetInfo.visible)
				{
					widgetInfo.widget->render(windowSize);
				}
			}
		}

		WidgetContainerInternal::WidgetInfo::WidgetInfo (OwnPtr<WidgetInternal> newWidget)
		{
			widget = newWidget;
			active = true;
			visible = true;
		}
	}
}

