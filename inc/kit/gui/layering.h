#pragma once

#include <kit/widget_container.h>

namespace kit
{
	class Layering : public WidgetContainer
	{
	public:
		// Shortcut for addLayer. Type must be a app::Widget.
		template <typename Type> std::shared_ptr<Type> addLayer();

		void addLayer(std::shared_ptr<Widget> widget);
		void insertLayerBefore(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> beforeWidget);
		void removeLayer(std::shared_ptr<Widget> widget);

	private:
		void updateWidgetBounds() override;

		std::list<std::shared_ptr<Widget>> layers;
	};

	// Template Implementatino

	namespace app
	{
		template <typename Type>
		std::shared_ptr<Type> Layers::addLayer()
		{
			auto widgetT = std::make_shared<Type>();
			addLayer(widgetT);
			return widgetT;
		}
	}
}

