#pragma once

#include "widget_p.h"
#include <kit/gui/widget_container.h>
#include <map>
#include <list>

namespace kit
{
	namespace gui
	{
		class WidgetContainerP : virtual public WidgetContainer, virtual public WidgetP
		{
		public:
			void setHandleContainerEventFunction (std::function<bool (Event const &, bool)>) override;

			void setUpdateWidgetBoundsFunction (std::function<void ()>) override;

			void callUpdateWidgetBoundsFunction ();

			Recti getBounds () const override;

			void setPosition (Vector2i position) override;

			void setMaxSize (Vector2i maxSize) override;

			Ptr<Widget> insertWidgetBefore (WidgetType type, Ptr<Widget> beforeWidget) override;

			Ptr<Widget> addWidget (WidgetType type) override;

			void removeWidget (Ptr<Widget> widget) override;

			void setWidgetPlacement (Ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset) override;

			void setWidgetPlacementSize (Ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize) override;

			bool isWidgetActive (Ptr<Widget> widget) const override;

			void setWidgetActive (Ptr<Widget> widget, bool active) override;

			bool isWidgetVisible (Ptr<Widget> widget) const override;

			void setWidgetVisible (Ptr<Widget> widget, bool visible) override;

			bool handleEvent (Event const & event, bool cursorIsValid) override;

			void render (Vector2i windowSize) override;

		private:
			class WidgetInfo
			{
			public:
				WidgetInfo (OwnPtr<WidgetP>);

				OwnPtr<WidgetP> widget;
				bool active;
				bool visible;
			};

			OwnPtr<WidgetP> createWidget (WidgetType type);

			std::function<bool (Event const &, bool)> handleContainerEventFunction;
			std::function<void ()> updateWidgetBoundsFunction;
			Recti bounds;
			std::list<WidgetInfo> widgetInfos;
			std::map<Ptr<Widget>, std::list<WidgetInfo>::iterator> widgetLookup;
		};
	}
}

