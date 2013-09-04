#include "Entity.h"

Entity::Entity()
{
	position = Vector3f::zero();
	scale = Vector3f::filled(1);
	matrixInverse = matrix = Matrix44f::identity();
	matricesNeedUpdate = false;
}

Vector3f const & Entity::getPosition() const
{
	return position;
}

void Entity::setPosition(Vector3f newPosition)
{
	position = newPosition;
	matricesNeedUpdate = true;
}

Quaternionf const & Entity::getOrientation() const
{
	return orientation;
}

void Entity::setOrientation(Quaternionf newOrientation)
{
	orientation = newOrientation;
	matricesNeedUpdate = true;
}

Vector3f const & Entity::getScale() const
{
	return scale;
}

void Entity::setScale(Vector3f newScale)
{
	scale = newScale;
	matricesNeedUpdate = true;
}

Matrix44f const & Entity::getMatrix() const
{
	if(matricesNeedUpdate)
	{
		const_cast<Entity *>(this)->updateMatrices();
	}
	return matrix;
}

Matrix44f  const & Entity::getMatrixInverse() const
{
	if(matricesNeedUpdate)
	{
		const_cast<Entity *>(this)->updateMatrices();
	}
	return matrixInverse;
}

void Entity::updateMatrices()
{
	Matrix33f rot = orientation.getMatrix();
	matrix(0, 0) = rot(0, 0) * scale[0];
	matrix(1, 0) = rot(1, 0) * scale[0];
	matrix(2, 0) = rot(2, 0) * scale[0];
	matrix(0, 1) = rot(0, 1) * scale[1];
	matrix(1, 1) = rot(1, 1) * scale[1];
	matrix(2, 1) = rot(2, 1) * scale[1];
	matrix(0, 2) = rot(0, 2) * scale[2];
	matrix(1, 2) = rot(1, 2) * scale[2];
	matrix(2, 2) = rot(2, 2) * scale[2];
	matrix(0, 3) = position[0];
	matrix(1, 3) = position[1];
	matrix(2, 3) = position[2];
	matrixInverse(0, 0) = rot(0, 0) / scale[0];
	matrixInverse(1, 0) = rot(0, 1) / scale[1];
	matrixInverse(2, 0) = rot(0, 2) / scale[2];
	matrixInverse(0, 1) = rot(1, 0) / scale[0];
	matrixInverse(1, 1) = rot(1, 1) / scale[1];
	matrixInverse(2, 1) = rot(1, 2) / scale[2];
	matrixInverse(0, 2) = rot(2, 0) / scale[0];
	matrixInverse(1, 2) = rot(2, 1) / scale[1];
	matrixInverse(2, 2) = rot(2, 2) / scale[2];
	matrixInverse(0, 3) = (-position[0] * rot(0, 0) - position[1] * rot(1, 0) - position[2] * rot(2, 0)) / scale[0];
	matrixInverse(1, 3) = (-position[0] * rot(0, 1) - position[1] * rot(1, 1) - position[2] * rot(2, 1)) / scale[1];
	matrixInverse(2, 3) = (-position[0] * rot(0, 2) - position[1] * rot(1, 2) - position[2] * rot(2, 2)) / scale[2];
	matricesNeedUpdate = false;
}

