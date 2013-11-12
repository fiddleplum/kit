#include "Layering.h"

namespace Gui
{
	Layering::Layering()
	{
		bounds.min = bounds.max = Vector2i::zero();
	}

	void Layering::addLayer(std::shared_ptr<Widget> widget)
	{
		if(widget == nullptr)
		{
			throw std::runtime_error("Layering::addLayer, widget == nullptr");
		}
		layers.push_back(widget);
		widget->setMaxSize(bounds.getSize());
	}

	void Layering::insertLayerBefore(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> relativeTo)
	{
		if(widget == nullptr)
		{
			throw std::runtime_error("Layering::insertLayerBefore, widget == nullptr");
		}
		for(auto it = layers.begin(); it != layers.end(); it++)
		{
			if(*it == relativeTo)
			{
				layers.insert(it, widget);
				widget->setMaxSize(bounds.getSize());
				break;
			}
		}
	}

	void Layering::insertLayerAfter(std::shared_ptr<Widget> widget, std::shared_ptr<Widget> relativeTo)
	{
		if(widget == nullptr)
		{
			throw std::runtime_error("Layering::insertLayerAfter, widget == nullptr");
		}
		for(auto it = layers.begin(); it != layers.end(); it++)
		{
			if(*it == relativeTo)
			{
				it++;
				layers.insert(it, widget);
				widget->setMaxSize(bounds.getSize());
				break;
			}
		}
	}

	void Layering::removeLayer(std::shared_ptr<Widget> widget)
	{
		for(auto it = layers.begin(); it != layers.end(); it++)
		{
			if(*it == widget)
			{
				layers.erase(it);
				break;
			}
		}
	}

	Box2i Layering::getBounds() const
	{
		return bounds;
	}

	void Layering::setPosition(Vector2i position)
	{
		bounds.setMinKeepSize(position);
		for(std::shared_ptr<Widget> widget : layers)
		{
			widget->setPosition(position);
		}
	}

	void Layering::setMaxSize(Vector2i maxSize)
	{
		bounds.setSize(maxSize);
		for(std::shared_ptr<Widget> widget : layers)
		{
			widget->setMaxSize(maxSize);
		}
	}

	void Layering::handleEvent(Event const & event)
	{
		for(auto it = layers.rbegin(); it != layers.rend(); it++)
		{
			(*it)->handleEvent(event);
		}
	}

	void Layering::render()
	{
		for(auto it = layers.begin(); it != layers.end(); it++)
		{
			(*it)->render();
		}
	}
}

