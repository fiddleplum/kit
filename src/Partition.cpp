#include "Partition.h"

Partition::Partition()
{
	horizontal = true;
	updatePanels();
}

void Partition::setHorizontal()
{
	horizontal = true;
	updatePanels();
}

void Partition::setVertical()
{
	horizontal = false;
	updatePanels();
}

template <typename T> T * Partition::addPanel(unsigned int pixelSize, float percentageSize)
{
	updatePanels();
}

template <typename T> T * Partition::setPanelWidget(unsigned int index)
{
	updatePanels();
}

void Partition::setPanelSize(unsigned int index, unsigned int pixelSize, float percentageSize)
{
	updatePanels();
}

void Partition::removePanel(unsigned int index)
{
	updatePanels();
}

Box2i Partition::getBounds() const
{
}

void Partition::setPosition(Vector2i)
{
}

void Partition::setMaxSize(Vector2i)
{
	updatePanels();
}

void Partition::render() const
{
}

void Partition::updatePanels()
{
}

