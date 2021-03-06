#include "body.h"

namespace kit
{
	namespace Flat
	{
		Body::Body()
		{
			angularVelocity = 0;
			bounciness = -1; // ghost by default
			inverseMass = 0; // infinite mass
			density = 0;
			accumTorque = 0;
		}

		void Body::addForceAtPosition(Coord2f force)
		{
			accumForce += force;
		}

		void Body::addForce(Coord2f force, Coord2f pointRelativeToFrame)
		{
			accumForce += force;
			accumTorque += pointRelativeToFrame.cross2d(force);
		}

		void Body::clearForces()
		{
			accumForce.set(0, 0);
			accumTorque = 0.0f;
		}
	}
}

