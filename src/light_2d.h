#pragma once

#include "frame_2d.h"

namespace kit
{
	class Light2D
	{
	public:
		Light2D();

		Vector2f getPosition() const { return frame.getPosition(); }
		void setPosition(Vector2f position) { frame.setPosition(position); }

		float getOrientation() const { return frame.getOrientation(); }
		void setOrientation(float orientation) { frame.setOrientation(orientation); }

		Vector3f getColor() const { return color; }
		void setColor(Vector3f);

	private:
		Frame2D frame;
		Vector3f color;
	};
}

