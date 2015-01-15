#include "light_2d.h"

namespace kit
{
	Light2D::Light2D()
	{
		color.set(1, 1, 1);
	}

	void Light2D::setColor(Coord3f newColor)
	{
		color = newColor;
	}
}

