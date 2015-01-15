#include "scene_entity_impl.h"

SceneEntityImpl::SceneEntityImpl()
{
	worldToLocalTransform = localToWorldTransform = Matrix44f::identity();
	transformsNeedUpdate = false;
}

Coord3f const & SceneEntityImpl::getPosition() const
{
	return position;
}

void SceneEntityImpl::setPosition(Coord3f position_)
{
	position = position_;
	transformsNeedUpdate = true;
}

Quaternionf const & SceneEntityImpl::getOrientation() const
{
	return orientation;
}

void SceneEntityImpl::setOrientation(Quaternionf orientation_)
{
	orientation = orientation_;
	transformsNeedUpdate = true;
}

Matrix44f const & SceneEntityImpl::getLocalToWorldTransform() const
{
	if(transformsNeedUpdate)
	{
		const_cast<SceneEntityImpl *>(this)->updateTransforms();
	}
	return localToWorldTransform;
}

Matrix44f  const & SceneEntityImpl::getWorldToLocalTransform() const
{
	if(transformsNeedUpdate)
	{
		const_cast<SceneEntityImpl *>(this)->updateTransforms();
	}
	return worldToLocalTransform;
}

void SceneEntityImpl::updateTransforms()
{
	Matrix33f rot = orientation.getMatrix();
	localToWorldTransform(0, 0) = rot(0, 0);
	localToWorldTransform(1, 0) = rot(1, 0);
	localToWorldTransform(2, 0) = rot(2, 0);
	localToWorldTransform(0, 1) = rot(0, 1);
	localToWorldTransform(1, 1) = rot(1, 1);
	localToWorldTransform(2, 1) = rot(2, 1);
	localToWorldTransform(0, 2) = rot(0, 2);
	localToWorldTransform(1, 2) = rot(1, 2);
	localToWorldTransform(2, 2) = rot(2, 2);
	localToWorldTransform(0, 3) = position[0];
	localToWorldTransform(1, 3) = position[1];
	localToWorldTransform(2, 3) = position[2];
	worldToLocalTransform(0, 0) = rot(0, 0);
	worldToLocalTransform(1, 0) = rot(0, 1);
	worldToLocalTransform(2, 0) = rot(0, 2);
	worldToLocalTransform(0, 1) = rot(1, 0);
	worldToLocalTransform(1, 1) = rot(1, 1);
	worldToLocalTransform(2, 1) = rot(1, 2);
	worldToLocalTransform(0, 2) = rot(2, 0);
	worldToLocalTransform(1, 2) = rot(2, 1);
	worldToLocalTransform(2, 2) = rot(2, 2);
	worldToLocalTransform(0, 3) = (-position[0] * rot(0, 0) - position[1] * rot(1, 0) - position[2] * rot(2, 0));
	worldToLocalTransform(1, 3) = (-position[0] * rot(0, 1) - position[1] * rot(1, 1) - position[2] * rot(2, 1));
	worldToLocalTransform(2, 3) = (-position[0] * rot(0, 2) - position[1] * rot(1, 2) - position[2] * rot(2, 2));
	transformsNeedUpdate = false;
}

