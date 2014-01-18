#pragma once

#include "Widget.h"
#include <map>
#include <list>
#include <memory>

namespace Gui
{
	class WidgetContainer : public Widget
	{
	public:
		WidgetContainer();

		Box2i getBounds() const override;
		void setPosition(Vector2i position) override;
		void setMaxSize(Vector2i maxSize) override;

	protected:
		virtual void handleContainerEvent(Event const & event) {}; // handles container-wide events
		virtual void updateWidgetBounds() = 0; // updates the widget bounds after the container has moved or otherwise changed

		void insertWidgetBefore(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> beforeWidget);
		void addWidget(std::shared_ptr<Widget> widget);
		void removeWidget(std::shared_ptr<Widget> widget);
		void replaceWidget(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> newWidget);

		void setWidgetPlacement(std::shared_ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset);
		void setWidgetPlacementSize(std::shared_ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize);

		bool isWidgetVisible(std::shared_ptr<Widget> widget) const;
		bool isWidgetActive(std::shared_ptr<Widget> widget) const;
		void setWidgetVisible(std::shared_ptr<Widget> widget, bool visible); // The widget is/isn't rendered and handle events.
		void setWidgetActive(std::shared_ptr<Widget> widget, bool active); // The widget does/doesn't handle events.

	private:
		class WidgetInfo
		{
		public:
			WidgetInfo(std::shared_ptr<Widget>);

			std::shared_ptr<Widget> widget;
			bool active;
			bool visible;
		};

		void handleEvent(Event const & event) override;
		void render() override;

		Box2i bounds;
		std::list<WidgetInfo> widgetInfos;
		std::map<std::shared_ptr<Widget>, std::list<WidgetInfo>::iterator> widgetLookup;
	};
}

