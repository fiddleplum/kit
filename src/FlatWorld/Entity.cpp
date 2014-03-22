#include "Entity.h"
#include <cmath>

namespace FlatWorld
{
	Entity::Entity()
	{
		position = Vector2f::zero();
		orientation = 0;
		scale = Vector2f::filled(1);
		z = 0;
		matrixInverse = matrix = Matrix33f::identity();
		matricesNeedUpdate = false;
	}

	void Entity::setPosition(Vector2f newPosition)
	{
		position = newPosition;
		matricesNeedUpdate = true;
	}

	void Entity::setVelocity(Vector2f newVelocity)
	{
		velocity = newVelocity;
	}

	void Entity::setOrientation(float newOrientation)
	{
		orientation = newOrientation;
		matricesNeedUpdate = true;
	}

	void Entity::setAngularVelocity(float newAngularVelocity)
	{
		angularVelocity = newAngularVelocity;
	}

	void Entity::setScale(Vector2f newScale)
	{
		scale = newScale;
		matricesNeedUpdate = true;
	}

	void Entity::setZ(int newZ)
	{
		z = newZ;
	}

	void Entity::setSolid(bool newSolid)
	{
		solid = newSolid;
	}

	void Entity::setInverseMass(float newInverseMass)
	{
		inverseMass = newInverseMass;
	}

	void Entity::setDensity(float newDensity)
	{
		density = newDensity;
	}

	void Entity::addForceAtPosition(Vector2f force)
	{
		accumForce += force;
	}

	void Entity::addForce(Vector2f force, Vector2f point)
	{
		accumForce += force;
		accumTorque += (point - position).cross2d(force);
	}

	void Entity::clearForces()
	{
		accumForce = Vector2f::zero();
		accumTorque = 0.0f;
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
		float cosOrientation = std::cos(orientation);
		float sinOrientation = std::sin(orientation);
		matrix(0, 0) = cosOrientation * scale[0];
		matrix(1, 0) = sinOrientation * scale[0];
		matrix(0, 1) = -sinOrientation * scale[1];
		matrix(1, 1) = cosOrientation * scale[1];
		matrix(0, 2) = position[0];
		matrix(1, 2) = position[1];
		matrixInverse(0, 0) = cosOrientation / scale[0];
		matrixInverse(1, 0) = -sinOrientation / scale[1];
		matrixInverse(0, 1) = sinOrientation / scale[0];
		matrixInverse(1, 1) = cosOrientation / scale[1];
		matrixInverse(0, 2) = (-position[0] * cosOrientation - position[1] * sinOrientation) / scale[0];
		matrixInverse(1, 2) = (position[0] * sinOrientation - position[1] * cosOrientation) / scale[1];
		matricesNeedUpdate = false;
	}
}

