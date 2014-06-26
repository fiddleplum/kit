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
			void setHandleContainerEventFunction (std::function<void (Event const &)>) override;

			void setUpdateWidgetBoundsFunction (std::function<void ()>) override;

			void callUpdateWidgetBoundsFunction ();

			Recti getBounds () const override;

			void setPosition (Vector2i position) override;

			void setMaxSize (Vector2i maxSize) override;

			void insertWidgetBefore (UsePtr<Widget> widget, UsePtr<Widget> beforeWidget) override;

			void addWidget (UsePtr<Widget> widget) override;

			void removeWidget (UsePtr<Widget> widget) override;

			void setWidgetPlacement (UsePtr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset) override;

			void setWidgetPlacementSize (UsePtr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize) override;

			bool isWidgetActive (UsePtr<Widget> widget) const override;

			void setWidgetActive (UsePtr<Widget> widget, bool active) override;

			bool isWidgetVisible (UsePtr<Widget> widget) const override;

			void setWidgetVisible (UsePtr<Widget> widget, bool visible) override;

			void handleEvent (Event const & event) override;

			void render (Vector2i windowSize) override;

		private:
			class WidgetInfo
			{
			public:
				WidgetInfo (UsePtr<Widget>);

				UsePtr<WidgetP> widget;
				bool active;
				bool visible;
			};

			std::function<void (Event const &)> handleContainerEventFunction;
			std::function<void ()> updateWidgetBoundsFunction;
			Recti bounds;
			std::list<WidgetInfo> widgetInfos;
			std::map<UsePtr<Widget>, std::list<WidgetInfo>::iterator> widgetLookup;
		};
	}
}

