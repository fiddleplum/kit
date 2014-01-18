#include "Layering.h"

namespace Gui
{
	Layering::Layering()
	{
	}

	void Layering::addLayer(std::shared_ptr<Widget> widget)
	{
		Box2i bounds = getBounds();
		addWidget(widget);
		layers.insert(layers.end(), widget);
		widget->setPosition(bounds.min);
		widget->setMaxSize(bounds.getSize());
	}

	void Layering::insertLayerBefore(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> beforeWidget)
	{
		insertWidgetBefore(widget, beforeWidget);
		for(auto it = layers.begin(); it != layers.end(); it++)
		{
			if(*it == beforeWidget)
			{
				Box2i bounds = getBounds();
				layers.insert(it, widget);
				widget->setPosition(bounds.min);
				widget->setMaxSize(bounds.getSize());
				break;
			}
		}
	}

	void Layering::removeLayer(std::shared_ptr<Widget> widget)
	{
		removeWidget(widget);
		for(auto it = layers.begin(); it != layers.end(); it++)
		{
			if(*it == widget)
			{
				layers.erase(it);
				break;
			}
		}
	}

	void Layering::updateWidgetBounds()
	{
		Box2i bounds = getBounds();
		for(std::shared_ptr<Widget> widget : layers)
		{
			widget->setPosition(bounds.min);
			widget->setMaxSize(bounds.getSize());
		}
	}
}

