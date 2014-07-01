#pragma once

#include "../vector.h"

namespace kit
{
	namespace Flat
	{
		class Body
		{
		public:
			Body();

			Vector2f const & getVelocity() const { return velocity; }
			void setVelocity(Vector2f velocity) { this->velocity = velocity; }

			float const & getAngularVelocity() const { return angularVelocity; }
			void setAngularVelocity(float angularVelocity) { this->angularVelocity = angularVelocity; }

			float getBounciness() const { return bounciness; }
			void setBounciness(float bounciness) { this->bounciness = bounciness; }

			float getInverseMass() const { return inverseMass; }
			void setInverseMass(float inverseMass) { this->inverseMass = inverseMass; }

			float getDensity() const { return density; }
			void setDensity(float density) { this->density = density; }

			Vector2f getAccumForce() const { return accumForce; }
			float getAccumTorque() const { return accumTorque; }
			void addForceAtPosition(Vector2f force);
			void addForce(Vector2f force, Vector2f pointRelativeToFrame);
			void clearForces();

		private:
			// Velocities
			Vector2f velocity;
			float angularVelocity;

			// Properties
			float bounciness; // -1 means it is a ghost, 0 means it sticks, +1 means it is bounces perfectly
			float inverseMass;
			float density; // approx moment of inertia = mass^2 / density

			// Forces
			Vector2f accumForce;
			float accumTorque;
		};
	}
}

