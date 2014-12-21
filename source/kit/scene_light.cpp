#include "scene_light.h"

SceneLight::SceneLight()
{
	color.set(1, 1, 1);
}

Vector3f SceneLight::getColor() const
{
	return color;
}

void SceneLight::setColor(Vector3f newColor)
{
	color = newColor;
}

