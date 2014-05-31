#pragma once

#include <kit/gui/widget.h>
#include <kit/ptr.h>
#include <map>
#include <list>
#include <memory>
#include <functional>

namespace kit
{
	namespace gui
	{
		class WidgetContainer : public Widget
		{
		public:
			WidgetContainer ();

			void setHandleContainerEventFunction (std::function<bool (Event const &, bool)>); // handles container-wide events
			void setUpdateWidgetBoundsFunction (std::function<void ()>); // updates the widget bounds after the container has moved or otherwise changed

			Recti getBounds () const override;
			void setPosition (Vector2i position) override;
			void setMaxSize (Vector2i maxSize) override;

			template <typename WidgetType> Ptr<WidgetType> insertWidgetBefore (Ptr<Widget> beforeWidget);
			template <typename WidgetType> Ptr<WidgetType> addWidget ();
			template <typename WidgetType> Ptr<WidgetType> replaceWidget (Ptr<Widget> oldWidget);
			void removeWidget (Ptr<Widget> widget);

			void setWidgetPlacement (Ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset);
			void setWidgetPlacementSize (Ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize);

			bool isWidgetActive (Ptr<Widget> widget) const;
			void setWidgetActive (Ptr<Widget> widget, bool active); // The widget does/doesn't handle events.

			bool isWidgetVisible (Ptr<Widget> widget) const;
			void setWidgetVisible (Ptr<Widget> widget, bool visible); // The widget is/isn't rendered and handle events.

		private:
			class WidgetInfo
			{
			public:
				WidgetInfo (OwnPtr<Widget>);

				OwnPtr<Widget> widget;
				bool active; // does it handle events and is it rendered?
				bool visible; // is it rendered?
			};

			void insertWidgetBefore (OwnPtr<Widget> widget, Ptr<Widget> beforeWidget);
			void addWidget (OwnPtr<Widget> widget);
			void replaceWidget (OwnPtr<Widget> widget, Ptr<Widget> oldWidget);

			bool handleEvent(Event const & event, bool cursorIsValid) override;
			void render(Vector2i windowSize) override;

			std::function<bool (Event const &, bool)> handleContainerEventFunction;
			std::function<void ()> updateWidgetBoundsFunction;
			Recti bounds;
			std::list<WidgetInfo> widgetInfos;
			std::map<Ptr<Widget>, std::list<WidgetInfo>::iterator> widgetLookup;
		};

		// Template Implementation

		template <typename WidgetType> Ptr<WidgetType> WidgetContainer::insertWidgetBefore (Ptr<Widget> beforeWidget)
		{
			OwnPtr<WidgetType> widget (new WidgetType);
			insertWidgetBefore(widget, beforeWidget);
			return widget;
		}

		template <typename WidgetType> Ptr<WidgetType> WidgetContainer::addWidget ()
		{
			OwnPtr<WidgetType> widget (new WidgetType);
			addWidget(widget);
			return widget;
		}

		template <typename WidgetType> Ptr<WidgetType> WidgetContainer::replaceWidget (Ptr<Widget> oldWidget)
		{
			OwnPtr<WidgetType> widget (new WidgetType);
			replaceWidget(widget, oldWidget);
			return widget;
		}
	}
}

