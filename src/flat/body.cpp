#include "body.h"

namespace Flat
{
	Body::Body()
	{
		velocity = Vector2f::zero();
		angularVelocity = 0;
		bounciness = -1; // ghost by default
		inverseMass = 0; // infinite mass
		density = 0;
		accumForce = Vector2f::zero();
		accumTorque = 0;
	}

	void Body::addForceAtPosition(Vector2f force)
	{
		accumForce += force;
	}

	void Body::addForce(Vector2f force, Vector2f pointRelativeToFrame)
	{
		accumForce += force;
		accumTorque += pointRelativeToFrame.cross2d(force);
	}

	void Body::clearForces()
	{
		accumForce = Vector2f::zero();
		accumTorque = 0.0f;
	}
}

