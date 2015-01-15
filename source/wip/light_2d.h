#pragma once

#include "frame_2d.h"

namespace kit
{
	class Light2D
	{
	public:
		Light2D();

		Coord2f getPosition() const { return frame.getPosition(); }
		void setPosition(Coord2f position) { frame.setPosition(position); }

		float getOrientation() const { return frame.getOrientation(); }
		void setOrientation(float orientation) { frame.setOrientation(orientation); }

		Coord3f getColor() const { return color; }
		void setColor(Coord3f);

	private:
		Frame2D frame;
		Coord3f color;
	};
}

