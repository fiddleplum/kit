#pragma once

#include "WidgetContainer.h"

namespace Gui
{
	class Layering : public WidgetContainer
	{
	public:
		Layering();

		void addLayer(std::shared_ptr<Widget> widget);
		void insertLayerBefore(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> beforeWidget);
		void removeLayer(std::shared_ptr<Widget> widget);

	private:
		void updateWidgetBounds() override;

		std::list<std::shared_ptr<Widget>> layers;
	};
}

