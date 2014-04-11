#include "Light.h"

Light::Light()
{
	color.set(1, 1, 1);
}

Vector3f Light::getColor() const
{
	return color;
}

void Light::setColor(Vector3f newColor)
{
	color = newColor;
}

