#pragma once

#include "widget_container.h"

namespace kit
{
	namespace app
	{
		class Partition : public WidgetContainer
		{
		public:
			// Constructs the partition. Defaults to a horizontal stack of panels.
			Partition();

			// Sets the partition to horizontally stack panels like columns of a table.
			void setHorizontal();

			// Sets the partition to vertically stack panels like rows of a table.
			void setVertical();

			// Appends a panel to the stack. See setPanelWidget and setPanelSize for details on params.
			void addPanel(std::shared_ptr<Widget> widget, int pixelSize, float scaleSize);

			// Removes the panel at the specified index. If the panel does not exist, does nothing.
			void removePanel(int index);

			// Creates a widget set in the panel at the specified index.
			void setPanelWidget(int index, std::shared_ptr<Widget> widget);

			// Sets the panel size at the specified index according to: pixelSize + scaleSize * (pixels remaining after pixelSizes of all panels are taken into account).
			void setPanelSize(int index, int pixelSize, float scaleSize);

			// Inherited from Widget.
			virtual Recti getBounds() const override;
			virtual void setPosition(Vector2i) override;
			virtual void setMaxSize(Vector2i) override;
			virtual void handleEvent(Event const & event) override;
			virtual void render() override;

		private:
			class Panel
			{
			public:
				std::shared_ptr<Widget> widget;
				int pixelSize;
				float scaleSize;
				int actualSize;
			};
			void updateWidgetBounds() override;

			std::list<Panel> panels;
			int axis;
		};
	}
}

