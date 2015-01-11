#include "ptr.h"
#include "vector.h"
#include "interval.h"
#include "gui_element.h"

void d()
{
	Ptr<int> a;
	OwnPtr<int> b;

	Vector2i f = {3, 4};
	Vector2i g;
	f = g = {6, 7};

	Interval<3, float> d;
}

