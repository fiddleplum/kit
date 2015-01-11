#include "scene_light_impl.h"

SceneLightImpl::SceneLightImpl()
{
	color = {1, 1, 1};
}

Vector3f SceneLightImpl::getColor() const
{
	return color;
}

void SceneLightImpl::setColor(Vector3f color_)
{
	color = color_;
}

