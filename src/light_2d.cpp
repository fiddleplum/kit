#include "light_2d.h"

Light2D::Light2D()
{
	color.set(1, 1, 1);
}

void Light2D::setColor(Vector3f newColor)
{
	color = newColor;
}

