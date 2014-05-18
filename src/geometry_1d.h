#pragma once

#include "vector.h"
#include "interval.h"

class Impact1D
{
public:
	bool valid; // is the impact valid
	float time; // time at which the impact occurs
	float point; // point at which the impact occurs
};

// Gets time and point of impact of moving ranges. pointAtImpact is only valid if timeAtImpact is exclusively between 0 and 1.
Impact1D getImpactOfMovingRanges(Rangef range1, float offset1, Rangef range2, float offset2);

