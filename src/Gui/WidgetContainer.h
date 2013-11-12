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
		void handleEvent(Event const & event) override;
		void render() override;

	protected:
		virtual void updateWidgetBounds() = 0;

		void insertWidgetBefore(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> beforeWidget);
		void appendWidget(std::shared_ptr<Widget> widget);
		void removeWidget(std::shared_ptr<Widget> widget);

		void setWidgetPlacement(std::shared_ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i pixelOffset);
		void setWidgetPlacementSize(std::shared_ptr<Widget> widget, Vector2f fractionalSize, Vector2i pixelSize);

	private:
		void handleAddsAndRemoves();

		Box2i bounds;
		std::list<std::shared_ptr<Widget>> widgets;
		std::map<std::shared_ptr<Widget>, std::list<std::shared_ptr<Widget>>::iterator> widgetLookup;
	};
}

