#include "geometry_2d.h"

Rectf Polygon::getBoundingRect() const
{
	Rectf boundingRect;
	if(vertices.size() > 0)
	{
		boundingRect.min = boundingRect.max = vertices[0];
		for(unsigned int i = 1; i < vertices.size(); i++)
		{
			boundingRect.extendTo(vertices[i]);
		}
	}
	else
	{
		boundingRect.min = boundingRect.max = Vector2f::zero();
	}
	return boundingRect;
}

