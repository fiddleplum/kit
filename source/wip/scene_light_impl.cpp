#include "scene_light_impl.h"

SceneLightImpl::SceneLightImpl()
{
	color = {1, 1, 1};
}

Coord3f SceneLightImpl::getColor() const
{
	return color;
}

void SceneLightImpl::setColor(Coord3f color_)
{
	color = color_;
}

