#pragma once

#include "Widget.h"
#include <vector>

class Partition : public Widget
{
public:
	/// Constructs the partition. Defaults to a horizontal stack of panels.
	Partition();

	/// Sets the partition to horizontally stack panels like columns of a table.
	void setHorizontal();

	/// Sets the partition to vertically stack panels like rows of a table.
	void setVertical();

	/// Appends a panel to the stack. See setPanelWidget and setPanelSize for details on params.
	template <typename T> T * addPanel(int pixelSize, float percentageSize);

	/// Creates a widget set in the panel at the specified index.
	template <typename T> T * setPanelWidget(int index);

	/// Sets the panel size at the specified index according to: pixelSize + percentageSize * (pixels remaining after pixelSizes of all panels are taken into account).
	void setPanelSize(int index, int pixelSize, float percentageSize);

	/// Removes the panel at the specified index. If the panel does not exist, does nothing.
	void removePanel(int index);

	/// Inherited from Widget.
	virtual Box2i getBounds() const override;

	/// Inherited from Widget.
	virtual void setPosition(Vector2i) override;

	/// Inherited from Widget.
	virtual void setMaxSize(Vector2i) override;

	/// Inherited from Widget.
	virtual void render() const override;

private:
	class Panel
	{
	public:
		Widget * widget;
		int pixelSize;
		float percentageSize;
		int actualSize;
	};
	void updatePanels();
	std::vector<Panel> panels;
	int axis;
	Box2i bounds;
};

