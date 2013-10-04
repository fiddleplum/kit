#pragma once

#include "Widget.h"
#include <vector>
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
		void addWidget(std::shared_ptr<Widget> widget, Vector2f externalFractionalOffset, Vector2f internalFractionalOffset, Vector2i offset);
		void setWidgetPlacementSize(std::shared_ptr<Widget> widget, Vector2f fractionalOffset, Vector2i offset);
		void removeWidget(std::shared_ptr<Widget> widget);

	private:
		class WidgetPlacement
		{
		public:
			std::shared_ptr<Widget> widget;
			Vector2f externalFractionalOffset; // relative to container size
			Vector2f internalFractionalOffset; // relative to widget size
			Vector2i offset; // in pixels
			Vector2f fractionalSizeOffset; // relative to container size
			Vector2i sizeOffset; // in pixels
		};

		void setWidgetBounds(WidgetPlacement & widgetPlacement);

		Box2i bounds;
		std::vector<WidgetPlacement> widgetPlacements;
	};
}

