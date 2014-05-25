#pragma once

#include <kit/vector.h>
#include "quaternion.h"
#include "matrix.h"

namespace kit
{
	class SceneEntity
	{
	public:
		SceneEntity();
		virtual ~SceneEntity() {}

		Vector3f const & getPosition() const;
		virtual void setPosition(Vector3f);

		Quaternionf const & getOrientation() const;
		virtual void setOrientation(Quaternionf);

		Vector3f const & getScale() const;
		virtual void setScale(Vector3f);

		Matrix44f const & getMatrix() const;
		Matrix44f const & getMatrixInverse() const;

	private:
		void updateMatrices();

		Vector3f position;
		Quaternionf orientation;
		Vector3f scale;
		bool matricesNeedUpdate;
		Matrix44f matrix;
		Matrix44f matrixInverse;
	};
}

