#pragma once

#include "Widget.h"
#include <vector>
#include <memory>

namespace Gui
{
	class Layering : public Widget
	{
	public:
		Layering();

		void addLayer(std::shared_ptr<Widget> widget);
		void insertLayerBefore(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> relativeTo);
		void insertLayerAfter(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> relativeTo);
		void removeLayer(std::shared_ptr<Widget> widget);

		Box2i getBounds() const override;
		void setPosition(Vector2i position) override;
		void setMaxSize(Vector2i maxSize) override;
		void handleEvent(Event const & event) override;
		void render() override;
		
	private:
		Box2i bounds;
		std::vector<std::shared_ptr<Widget>> layers;
	};
}

