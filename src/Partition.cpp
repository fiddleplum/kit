#include "Partition.h"

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

template <typename T> T * Partition::addPanel(int pixelSize, float percentageSize)
{
	Panel panel;
	panel.widget = new T;
	panel.pixelSize = pixelSize;
	panel.percentageSize = percentageSize;
	updatePanels();
	return widget;
}

template <typename T> T * Partition::setPanelWidget(int index)
{
	if(index >= panels.size())
	{
		throw std::exception();
	}
	delete panels[index].widget;
	panels[index].widget = new T;
	updatePanels();
}

void Partition::setPanelSize(int index, int pixelSize, float percentageSize)
{
	if(index >= (signed)panels.size())
	{
		throw std::exception();
	}
	panels[index].pixelSize = pixelSize;
	panels[index].percentageSize = percentageSize;
	updatePanels();
}

void Partition::removePanel(int index)
{
	if(index >= (signed)panels.size())
	{
		throw std::exception();
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

void Partition::render() const
{
	for(Panel const & panel : panels)
	{
		panel.widget->render();
	}
}

void Partition::updatePanels()
{
	int remainingSize = bounds.getSize()[axis];
	for(Panel & panel : panels)
	{
		panel.actualSize = std::min(panel.pixelSize, remainingSize);
		remainingSize -= panel.actualSize;
	}
	for(Panel & panel : panels)
	{
		panel.actualSize += (int)(panel.percentageSize * remainingSize);
		Vector2i size = getBounds().getSize();
		size[axis] = panel.actualSize;
		panel.widget->setMaxSize(size);
	}
}

