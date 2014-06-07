#pragma once

#include "widget_internal.h"
#include <kit/gui/widget_container.h>
#include <map>
#include <list>

namespace kit
{
	namespace gui
	{
		class WidgetContainerInternal : virtual public WidgetContainer, virtual public WidgetInternal
		{
		public:
			WidgetContainerInternal ();

			void setHandleContainerEventFunction (std::function<bool (Event const &, bool)>);

			void setUpdateWidgetBoundsFunction (std::function<void ()>);

			Recti getBounds () const override;

			void setPosition (Vector2i position);

			void setMaxSize (Vector2i maxSize);

			Ptr<Widget> insertWidgetBefore (WidgetType type, Ptr<Widget> beforeWidget);

			Ptr<Widget> addWidget (WidgetType type);

			void removeWidget (Ptr<Widget> widget);

			void setWidgetPlacement (Ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset);

			void setWidgetPlacementSize (Ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize);

			bool isWidgetActive (Ptr<Widget> widget) const;

			void setWidgetActive (Ptr<Widget> widget, bool active);

			bool isWidgetVisible (Ptr<Widget> widget) const;

			void setWidgetVisible (Ptr<Widget> widget, bool visible);

			bool handleEvent (Event const & event, bool cursorIsValid);

			void render (Vector2i windowSize);

			class WidgetInfo
			{
			public:
				WidgetInfo (OwnPtr<WidgetInternal>);

				OwnPtr<WidgetInternal> widget;
				bool active; // does it handle events and is it rendered?
				bool visible; // is it rendered?
			};

			std::function<bool (Event const &, bool)> handleContainerEventFunction;
			std::function<void ()> updateWidgetBoundsFunction;
			Recti bounds;
			std::list<WidgetInfo> widgetInfos;
			std::map<Ptr<Widget>, std::list<WidgetInfo>::iterator> widgetLookup;
		};
	}
}

