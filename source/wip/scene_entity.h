#pragma once

#include "coord.h"
#include "matrix.h"
#include "quaternion.h"

class SceneEntity
{
public:
	virtual Coord3f const & getPosition() const = 0;

	virtual void setPosition(Coord3f position) = 0;

	virtual Quaternionf const & getOrientation() const = 0;

	virtual void setOrientation(Quaternionf orientation) = 0;

	virtual Matrix44f const & getLocalToWorldTransform() const = 0;

	virtual Matrix44f const & getWorldToLocalTransform() const = 0;
};

