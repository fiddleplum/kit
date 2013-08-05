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
	template <typename T> T * addPanel(unsigned int pixelSize, float percentageSize);

	/// Creates a widget set in the panel at the specified index.
	template <typename T> T * setPanelWidget(unsigned int index);

	/// Sets the panel size at the specified index according to: pixelSize + percentageSize * (pixels remaining after pixelSizes of all panels are taken into account).
	void setPanelSize(unsigned int index, unsigned int pixelSize, float percentageSize);

	/// Removes the panel at the specified index. If the panel does not exist, does nothing.
	void removePanel(unsigned int index);

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
		unsigned int pixels;
		float percentage;
	};
	void updatePanels();
	std::vector<Panel> panels;
	bool horizontal;
};

