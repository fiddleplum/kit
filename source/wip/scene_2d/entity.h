#pragma once

#include "../Vector.h"
#include "../Quaternion.h"
#include "../Matrix.h"

namespace Scene
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity() {}

		Vector2f const & getPosition() const;
		virtual void setPosition(Vector2f);

		float const & getOrientation() const;
		virtual void setOrientation(float);

		Vector2f const & getScale() const;
		virtual void setScale(Vector2f);

		Matrix33f const & getMatrix() const;
		Matrix33f  const & getMatrixInverse() const;

		void updateMatrices();

		Vector2f position;
		float orientation;
		Vector2f scale;
		bool matricesNeedUpdate;
		Matrix33f matrix;
		Matrix33f matrixInverse;
	};
}

