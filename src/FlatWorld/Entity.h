#pragma once

#include "../Vector.h"
#include "../Matrix.h"
#include "../Interval.h"

namespace FlatWorld
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity() {}

		Vector2f const & getPosition() const { return position; }
		virtual void setPosition(Vector2f);

		Vector2f const & getVelocity() const { return velocity; }
		virtual void setVelocity(Vector2f);

		float const & getOrientation() const { return orientation; }
		virtual void setOrientation(float); // in radians

		float const & getAngularVelocity() const { return angularVelocity; }
		virtual void setAngularVelocity(float); // in radians

		Vector2f const & getScale() const { return scale; }
		virtual void setScale(Vector2f);

		int getZ() const { return z; }
		virtual void setZ(int);

		bool isSolid() const { return solid; }
		virtual void setSolid(bool);

		float getInverseMass() const { return inverseMass; }
		virtual void setInverseMass(float);

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

		Matrix33f const & getMatrix() const;
		Matrix33f const & getMatrixInverse() const;

	private:
		void updateMatrices();

		Vector2f position;
		Vector2f velocity;
		float orientation;
		float angularVelocity;
		Vector2f scale;
		int z;

		bool solid;
		float inverseMass;
		float density; // moment of inertia = mass^2 / density
		Vector2f accumForce;
		float accumTorque;

		Matrix33f matrix;
		Matrix33f matrixInverse;
		bool matricesNeedUpdate;
	};
}

