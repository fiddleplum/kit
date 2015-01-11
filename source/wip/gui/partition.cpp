#include "partition.h"

namespace kit
{
	namespace app
	{
		Partition::Partition()
		{
			axis = 0;
		}

		void Partition::setHorizontal()
		{
			axis = 0;
			updateWidgetBounds();
		}

		void Partition::setVertical()
		{
			axis = 1;
			updateWidgetBounds();
		}

		void Partition::addPanel(std::shared_ptr<Widget> widget, int pixelSize, float scaleSize)
		{
			addWidget(widget);
			Panel panel;
			panel.widget = widget;
			panel.pixelSize = pixelSize;
			panel.scaleSize = scaleSize;
			panels.insert(panels.end(), panel);
			updateWidgetBounds();
		}

		void Partition::removePanel(int index)
		{
			int i = 0;
			for(auto it = panels.begin(); it != panels.end(); it++)
			{
				if(i == index)
				{
					removeWidget(it->widget);
					panels.erase(it);
					updateWidgetBounds();
					break;
				}
				i++;
			}
		}

		void Partition::setPanelWidget(int index, std::shared_ptr<Widget> widget)
		{
			int i = 0;
			for(auto it = panels.begin(); it != panels.end(); it++)
			{
				if(i == index)
				{
					removeWidget(it->widget);
					addWidget(widget);
					it->widget = widget;
					updateWidgetBounds();
					break;
				}
				i++;
			}
		}

		void Partition::setPanelSize(int index, int pixelSize, float scaleSize)
		{
			int i = 0;
			for(auto it = panels.begin(); it != panels.end(); it++)
			{
				if(i == index)
				{
					it->pixelSize = pixelSize;
					it->scaleSize = scaleSize;
					updateWidgetBounds();
					break;
				}
				i++;
			}
		}

		void Partition::updateWidgetBounds()
		{
			Recti bounds = getBounds();
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
			Vector2i size = bounds.getSize();
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
}

