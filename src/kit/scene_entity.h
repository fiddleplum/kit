#pragma once

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

class SceneEntity
{
public:
	SceneEntity();

	Vector3f const & getPosition() const;

	virtual void setPosition(Vector3f position);

	Quaternionf const & getOrientation() const;

	virtual void setOrientation(Quaternionf orientation);

	Matrix44f const & getLocalToWorldTransform() const;

	Matrix44f const & getWorldToLocalTransform() const;

private:
	void updateTransforms();

	Vector3f position;
	Quaternionf orientation;
	bool transformsNeedUpdate;
	Matrix44f localToWorldTransform;
	Matrix44f worldToLocalTransform;
};

