#pragma once

#include <vector>

class Widget;

class WidgetContainer
{
public:
	WidgetContainer();

	~WidgetContainer();

	template <typename T> T * addWidget();

	void removeWidget(Widget * widget);

private:
	std::vector<Widget *> widgets;
};

