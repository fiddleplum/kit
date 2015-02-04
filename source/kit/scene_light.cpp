#include "scene_light.h"

SceneLight::SceneLight()
{
	color = {1, 1, 1};
}

Coord3f SceneLight::getColor() const
{
	return color;
}

void SceneLight::setColor(Coord3f color_)
{
	color = color_;
}

