#pragma once

#include "sprite_p.h"
#include "button_p.h"
#include "viewport_p.h"
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

			Ptr<Sprite> addSprite () override;

			Ptr<Button> addButton () override;

			Ptr<Viewport> addViewport () override;

			void removeWidget (Ptr<Widget> widget) override;

			void reinsertWidget (Ptr<Widget> widget, Ptr<Widget> beforeWidget) override;

			void setWidgetPlacement (Ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset) override;

			void setWidgetPlacementSize (Ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize) override;

			bool isWidgetActive (Ptr<Widget> widget) const override;

			void setWidgetActive (Ptr<Widget> widget, bool active) override;

			bool isWidgetVisible (Ptr<Widget> widget) const override;

			void setWidgetVisible (Ptr<Widget> widget, bool visible) override;

			void handleEvent (Event const & event) override;

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

			void addWidget (OwnPtr<WidgetP> widget);

			std::function<void (Event const &)> handleContainerEventFunction;
			std::function<void ()> updateWidgetBoundsFunction;
			Recti bounds;
			std::list<WidgetInfo> widgetInfos;
			std::map<Ptr<Widget>, std::list<WidgetInfo>::iterator> widgetLookup;
		};
	}
}

