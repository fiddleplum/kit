#include "geometry_1d.h"
#include <algorithm>

Impact1D getImpactOfMovingRanges(Rangef range1, float offset1, Rangef range2, float offset2)
{
	Impact1D impact;
	if(range1.max < range2.min)
	{
		if(offset1 > offset2)
		{
			impact.time = (range2.min - range1.max) / (offset1 - offset2);
			impact.point = range1.max + offset1 * impact.time;
			impact.valid = true;
		}
		else
		{
			impact.valid = false;
		}
	}
	else if(range2.max < range1.min)
	{
		if(offset2 > offset1)
		{
			impact.time = (range1.min - range2.max) / (offset2 - offset1);
			impact.point = range2.max + offset2 * impact.time;
			impact.valid = true;
		}
		else
		{
			impact.valid = false;
		}
	}
	else
	{
		impact.time = 0; // already intersecting
		impact.point = (std::max(range1.min, range2.min) + std::min(range1.max, range2.max)) / 2.0f;
		impact.valid = true;
	}
	return impact;
}

