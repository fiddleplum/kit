#include "widget_container.h"
#include <algorithm>

namespace kit
{
	namespace gui
	{
		void WidgetContainer::setHandleContainerEventFunction (std::function<void (Event const &)> newHandleContainerEventFunction)
		{
			handleContainerEventFunction = newHandleContainerEventFunction;
		}

		void WidgetContainer::setUpdateWidgetBoundsFunction (std::function<void ()> newUpdateWidgetBoundsFunction)
		{
			updateWidgetBoundsFunction = newUpdateWidgetBoundsFunction;
		}

		void WidgetContainer::callUpdateWidgetBoundsFunction ()
		{
			if(updateWidgetBoundsFunction)
			{
				updateWidgetBoundsFunction();
			}
		}

		Recti WidgetContainer::getBounds () const
		{
			return bounds;
		}

		void WidgetContainer::setPosition (Vector2i position)
		{
			bounds.setMinKeepSize(position);
			if(updateWidgetBoundsFunction)
			{
				updateWidgetBoundsFunction();
			}
		}

		void WidgetContainer::setMaxSize (Vector2i maxSize)
		{
			bounds.setSize(maxSize);
			if(updateWidgetBoundsFunction)
			{
				updateWidgetBoundsFunction();
			}
		}

		Ptr<Sprite> WidgetContainer::addSprite ()
		{
			OwnPtr<Sprite> sprite (new Sprite);
			addWidget(sprite);
			return sprite;
		}

		Ptr<Button> WidgetContainer::addButton ()
		{
			OwnPtr<Button> button (new Button);
			addWidget(button);
			return button;
		}

		Ptr<Viewport> WidgetContainer::addViewport ()
		{
			OwnPtr<Viewport> viewport (new Viewport);
			addWidget(viewport);
			return viewport;
		}

		void WidgetContainer::removeWidget (Ptr<Widget> widget)
		{
			if(widget.isValid())
			{
				widgetsToRemove.insert(widget);
			}
		}

		void WidgetContainer::reinsertWidget (Ptr<Widget> widget, Ptr<Widget> beforeWidget)
		{
			auto oldWidgetIterator = widgetLookup.find(widget);
			if(oldWidgetIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainer::reinsertWidget, widget not found");
			}
			if(widget == beforeWidget)
			{
				return;
			}
			WidgetInfo wi = *oldWidgetIterator->second;
			std::map<Ptr<Widget>, std::list<WidgetInfo>::iterator>::iterator beforeWidgetIterator;
			if(beforeWidget.isValid())
			{
				beforeWidgetIterator = widgetLookup.find(beforeWidget);
				if(beforeWidgetIterator == widgetLookup.end())
				{
					throw std::runtime_error ("WidgetContainer::reinsertWidget, beforeWidget not found");
				}
			}
			else
			{
				beforeWidgetIterator = widgetLookup.end();
			}
			removeWidget(widget); // won't delete it, since the wi has a copy of the OwnPtr.			
			auto newWidgetIterator = widgetInfos.insert(beforeWidgetIterator->second, wi);
			widgetLookup[widget] = newWidgetIterator;
		}

		void WidgetContainer::setWidgetPlacement (Ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset)
		{
			Vector2i position = bounds.min;

			position = Vector2i(externalFractionalOffset.scale(bounds.getSize())) - Vector2i(internalFractionalOffset.scale(widget->getBounds().getSize())) + pixelOffset;
			widget->setPosition(position);
		}

		void WidgetContainer::setWidgetPlacementSize (Ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize)
		{
			Vector2i position = widget->getBounds().min;
			Vector2i maxSize = Vector2i(fractionalSize.scale(bounds.getSize())) + pixelSize;
			maxSize[0] = std::min(maxSize[0], bounds.getSize()[0] - position[0]);
			maxSize[1] = std::min(maxSize[1], bounds.getSize()[1] - position[1]);
			widget->setMaxSize(maxSize);
		}

		bool WidgetContainer::isWidgetActive (Ptr<Widget> widget) const
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainer::setWidgetVisible, widget not found");
			}
			return lookupIterator->second->active;
		}

		void WidgetContainer::setWidgetActive (Ptr<Widget> widget, bool active)
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainer::setWidgetActive, widget not found");
			}
			lookupIterator->second->active = active;
		}

		bool WidgetContainer::isWidgetVisible (Ptr<Widget> widget) const
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainer::setWidgetVisible, widget not found");
			}
			return lookupIterator->second->visible;
		}

		void WidgetContainer::setWidgetVisible (Ptr<Widget> widget, bool visible)
		{
			auto lookupIterator = widgetLookup.find(widget);
			if(lookupIterator == widgetLookup.end())
			{
				throw std::runtime_error ("WidgetContainer::setWidgetVisible, widget not found");
			}
			lookupIterator->second->visible = visible;
		}

		void WidgetContainer::handleEvent (Event const & event)
		{
			if(event.type == Event::Update)
			{
				for(Ptr<Widget> widget : widgetsToRemove)
				{
					auto lookupIterator = widgetLookup.find(widget);
					if(lookupIterator != widgetLookup.end())
					{
						widgetInfos.erase(lookupIterator->second);
						widgetLookup.erase(lookupIterator);
					}
				}
				widgetsToRemove.clear();
			}
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

		void WidgetContainer::render (Vector2i windowSize)
		{
			for(auto const & widgetInfo : widgetInfos)
			{
				if(widgetInfo.active && widgetInfo.visible)
				{
					widgetInfo.widget->render(windowSize);
				}
			}
		}

		WidgetContainer::WidgetInfo::WidgetInfo (OwnPtr<Widget> newWidget)
		{
			widget = newWidget;
			active = true;
			visible = true;
		}

		void WidgetContainer::addWidget (OwnPtr<Widget> widget)
		{
			auto widgetIterator = widgetInfos.insert(widgetInfos.end(), WidgetInfo(widget));
			widgetLookup[widget] = widgetIterator;
			widget->setPosition(bounds.min);
			widget->setMaxSize(bounds.getSize());
		}
	}
}

