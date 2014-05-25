#pragma once

#include <kit/vector.h>
#include "matrix.h"

namespace kit
{
	class Frame2D
	{
	public:
		Frame2D();
		virtual ~Frame2D() {}

		Vector2f getPosition() const { return position; }
		void setPosition(Vector2f);

		float getOrientation() const { return orientation; }
		void setOrientation(float);

		Matrix33f const & getTransform() const;
		Matrix33f const & getTransformInv() const;

	private:
		void updateTransforms();

		Vector2f position;
		float orientation;
		Matrix33f transform;
		Matrix33f transformInv;
		bool transformsDirty;
	};
}

