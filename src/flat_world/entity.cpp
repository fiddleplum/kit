#include "entity.h"
#include <cmath>

namespace FlatWorld
{
	Entity::Entity()
	{
		velocity = Vector2f::zero();
		angularVelocity = 0;
		solid = false;
		inverseMass = 0;
		density = 0;
		accumForce = Vector2f::zero();
		accumTorque = 0;
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
		accumTorque += (point - getPosition()).cross2d(force);
	}

	void Entity::clearForces()
	{
		accumForce = Vector2f::zero();
		accumTorque = 0.0f;
	}
}

