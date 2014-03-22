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

		Vector3f const & getPosition() const;
		virtual void setPosition(Vector3f);

		Quaternionf const & getOrientation() const;
		virtual void setOrientation(Quaternionf);

		Vector3f const & getScale() const;
		virtual void setScale(Vector3f);

		Matrix44f const & getMatrix() const;
		Matrix44f const & getMatrixInverse() const;

		void updateMatrices();

		Vector3f position;
		Quaternionf orientation;
		Vector3f scale;
		bool matricesNeedUpdate;
		Matrix44f matrix;
		Matrix44f matrixInverse;
	};
}
