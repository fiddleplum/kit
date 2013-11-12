#include "Partition.h"

namespace Gui
{
	Partition::Partition()
	{
		axis = 0;
		updatePanels();
	}

	void Partition::setHorizontal()
	{
		axis = 0;
		updatePanels();
	}

	void Partition::setVertical()
	{
		axis = 1;
		updatePanels();
	}

	void Partition::addPanel(std::shared_ptr<Widget> widget, int pixelSize, float scaleSize)
	{
		Panel panel;
		panel.widget = widget;
		panel.pixelSize = pixelSize;
		panel.scaleSize = scaleSize;
		panels.push_back(panel);
		updatePanels();
	}

	void Partition::setPanelWidget(int index, std::shared_ptr<Widget> widget)
	{
		if(index >= (signed)panels.size())
		{
			throw std::runtime_error("Index for Partition is out of range.");
		}
		panels[index].widget = widget;
		updatePanels();
	}

	void Partition::setPanelSize(int index, int pixelSize, float scaleSize)
	{
		if(index >= (signed)panels.size())
		{
			throw std::runtime_error("Index for Partition is out of range.");
		}
		panels[index].pixelSize = pixelSize;
		panels[index].scaleSize = scaleSize;
		updatePanels();
	}

	void Partition::removePanel(int index)
	{
		if(index >= (signed)panels.size())
		{
			throw std::runtime_error("Index for Partition is out of range.");
		}
		panels.erase(panels.begin() + index);
		updatePanels();
	}

	Box2i Partition::getBounds() const
	{
		return bounds;
	}

	void Partition::setPosition(Vector2i position)
	{
		bounds.setMinKeepSize(position);
	}

	void Partition::setMaxSize(Vector2i maxSize)
	{
		bounds.setSize(maxSize);
		updatePanels();
	}

	void Partition::handleEvent(Event const & event)
	{
		for(Panel const & panel : panels)
		{
			if(panel.widget != nullptr)
			{
				panel.widget->handleEvent(event);
			}
		}
	}

	void Partition::render()
	{
		for(Panel const & panel : panels)
		{
			if(panel.widget != nullptr)
			{
				panel.widget->render();
			}
		}
	}

	void Partition::updatePanels()
	{
		int remainingSize = bounds.getSize()[axis];
		// fixed pixels
		for(Panel & panel : panels)
		{
			panel.actualSize = std::min(panel.pixelSize, remainingSize);
			remainingSize -= panel.actualSize;
		}
		// percentage pixels
		int scaleSizeOfOne = remainingSize;
		for(Panel & panel : panels)
		{
			int sizeAdjustment = std::min((int)(panel.scaleSize * scaleSizeOfOne), remainingSize);
			panel.actualSize += sizeAdjustment;
			remainingSize -= sizeAdjustment;
		}
		// remaining pixels, because of rounding
		for(Panel & panel : panels)
		{
			if(panel.scaleSize != 0) // if it has some scaling (we don't want to add pixels on non-scaled panels)
			{
				panel.actualSize++;
				remainingSize--;
			}
			if(remainingSize == 0)
			{
				break;
			}
		}
		// adjust widget sizes
		Vector2i size = getBounds().getSize();
		for(Panel & panel : panels)
		{
			size[axis] = panel.actualSize;
			if(panel.widget != nullptr)
			{
				panel.widget->setMaxSize(size);
			}
		}
	}
}

