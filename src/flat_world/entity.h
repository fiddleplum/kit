#pragma once

#include "../entity_2d.h"

namespace FlatWorld
{
	class Entity : public Entity2D
	{
	public:
		Entity();
		virtual ~Entity() {}

		Vector2f const & getVelocity() const { return velocity; }
		void setVelocity(Vector2f newVelocity) { velocity = newVelocity; }

		float const & getAngularVelocity() const { return angularVelocity; }
		void setAngularVelocity(float newAngularVelocity) { angularVelocity = newAngularVelocity; }

		bool isSolid() const { return solid; }
		void setSolid(bool);

		float getInverseMass() const { return inverseMass; }
		void setInverseMass(float);

		float getDensity() const { return density; }
		void setDensity(float);

		Vector2f getAccumForce() const { return accumForce; }
		float getAccumTorque() const { return accumTorque; }
		void addForceAtPosition(Vector2f force);
		void addForce(Vector2f force, Vector2f point);
		void clearForces();

		//virtual bool contains(Vector2f);
		//virtual bool intersects(Entity *);
		//virtual Box2f getBoundingBox() const;

	private:
		Vector2f velocity;
		float angularVelocity;

		bool solid;
		float inverseMass;
		float density; // moment of inertia = mass^2 / density
		Vector2f accumForce;
		float accumTorque;
	};
}

