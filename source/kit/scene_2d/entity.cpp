#include "Entity.h"

namespace Scene2D
{
	Entity::Entity()
	{
		position = Vector2f::zero();
		scale = Vector2f::filled(1);
		matrixInverse = matrix = Matrix33f::identity();
		matricesNeedUpdate = false;
	}

	Vector2f const & Entity::getPosition() const
	{
		return position;
	}

	void Entity::setPosition(Vector2f newPosition)
	{
		position = newPosition;
		matricesNeedUpdate = true;
	}

	float const & Entity::getOrientation() const
	{
		return orientation;
	}

	void Entity::setOrientation(float newOrientation)
	{
		orientation = newOrientation;
		matricesNeedUpdate = true;
	}

	Vector2f const & Entity::getScale() const
	{
		return scale;
	}

	void Entity::setScale(Vector2f newScale)
	{
		scale = newScale;
		matricesNeedUpdate = true;
	}

	Matrix33f const & Entity::getMatrix() const
	{
		if(matricesNeedUpdate)
		{
			const_cast<Entity *>(this)->updateMatrices();
		}
		return matrix;
	}

	Matrix33f  const & Entity::getMatrixInverse() const
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
		matrix(0, 1) = rot(0, 1) * scale[1];
		matrix(1, 1) = rot(1, 1) * scale[1];
		matrix(0, 2) = position[0];
		matrix(1, 2) = position[1];
		matrixInverse(0, 0) = rot(0, 0) / scale[0];
		matrixInverse(1, 0) = rot(0, 1) / scale[1];
		matrixInverse(0, 1) = rot(1, 0) / scale[0];
		matrixInverse(1, 1) = rot(1, 1) / scale[1];
		matrixInverse(0, 2) = (-position[0] * rot(0, 0) - position[1] * rot(1, 0)) / scale[0];
		matrixInverse(1, 2) = (-position[0] * rot(0, 1) - position[1] * rot(1, 1)) / scale[1];
		matricesNeedUpdate = false;
	}
}

