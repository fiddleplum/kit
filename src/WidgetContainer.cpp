#include "WidgetContainer.h"
#include "Widget.h"
#include <algorithm>

WidgetContainer::WidgetContainer()
{
}

WidgetContainer::~WidgetContainer()
{
	for(Widget * widget : widgets)
	{
		delete widget;
	}
}

template <typename T> T * WidgetContainer::addWidget()
{
	Widget * widget = new T();
	widgets.push_back(widget);
	return widget;
}

void WidgetContainer::removeWidget(Widget * widget)
{
	auto it = std::find(widgets.begin(), widgets.end(), widget);
	if(it != widgets.end())
	{
		delete *it;
		widgets.erase(it);
	}
}

