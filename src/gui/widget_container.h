#pragma once

#include "sprite.h"
#include "button.h"
#include "viewport.h"
#include <functional>
#include <list>
#include <map>

namespace kit
{
	namespace gui
	{
		class WidgetContainer : public Widget
		{
		public:
			// Handles container-wide events.
			void setHandleContainerEventFunction(std::function<void (Event const &)>);

			// Updates the widget bounds after the container has moved or otherwise changed.
			void setUpdateWidgetBoundsFunction(std::function<void ()>);

			// Manually calls the update widget bounds function.
			void callUpdateWidgetBoundsFunction ();

			// Gets the bounds of the container.
			Recti getBounds() const;

			// Sets the position of the container.
			void setPosition(Vector2i position);

			// Sets the maximum size this container can be.
			void setMaxSize(Vector2i maxSize);

			// Add a sprite into the container.
			Ptr<Sprite> addSprite();

			// Add a button into the container.
			Ptr<Button> addButton();

			// Add a viewport into the container.
			Ptr<Viewport> addViewport();

			// Remove a widget from the container.
			void removeWidget(Ptr<Widget> widget);

			// Reinsert the widget before another widget. If beforeWidget is null, the widget is reinserted at the end.
			void reinsertWidget(Ptr<Widget> widget, Ptr<Widget> beforeWidget);

			// Sets the position of the widget relative to this.
			void setWidgetPlacement(Ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset);

			// Sets the size of the widget relative to this.
			void setWidgetPlacementSize(Ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize);

			// Does the widget handle events and rendered?
			bool isWidgetActive(Ptr<Widget> widget) const;

			// Sets whether the widget handles events and rendered.
			void setWidgetActive(Ptr<Widget> widget, bool active);

			// Is the widget rendered?
			bool isWidgetVisible(Ptr<Widget> widget) const;

			// Sets whether the widget is rendered.
			void setWidgetVisible(Ptr<Widget> widget, bool visible);

			// Handles an event.
			void handleEvent (Event const & event) override;

			// Renders the widget container.
			void render (Vector2i windowSize) override;

		private:
			class WidgetInfo
			{
			public:
				WidgetInfo (OwnPtr<Widget>);

				OwnPtr<Widget> widget;
				bool active;
				bool visible;
			};

			void addWidget (OwnPtr<Widget> widget);

			std::function<void (Event const &)> handleContainerEventFunction;
			std::function<void ()> updateWidgetBoundsFunction;
			Recti bounds;
			std::list<WidgetInfo> widgetInfos;
			std::map<Ptr<Widget>, std::list<WidgetInfo>::iterator> widgetLookup;
		};
	}
}

