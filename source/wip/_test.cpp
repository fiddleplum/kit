#include "ptr.h"
#include "coord.h"
#include "interval.h"
#include "gui_element.h"

void d()
{
	Ptr<int> a;
	OwnPtr<int> b;

	Coord2i f = {3, 4};
	Coord2i g;
	f = g = {6, 7};

	Interval<3, float> d;
}

