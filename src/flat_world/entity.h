#pragma once

#include "../model_2d.h"

namespace FlatWorld
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity() {}

		Vector2f getPosition() const { return model->getPosition(); }
		void setPosition(Vector2f newPosition) { model->setPosition(newPosition); }

		float getOrientation() const { return model->getOrientation(); }
		void setOrientation(float newOrientation) { model->setPosition(newOrientation); }

		Vector2f getScale() const { return model->getScale(); }
		void setScale(Vector2f newScale) { model->setScale(newScale); }

		int getZ() const { return model->getZ(); }
		void setZ(int newZ) { model->setZ(newZ); }

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

		Matrix33f const & getTransform() const { return model->getTransform(); }
		Matrix33f const & getTransformInv() const { return model->getTransformInv(); }

	private:
		std::shared_ptr<Model2D> model;

		Vector2f velocity;
		float angularVelocity;

		bool solid;
		float inverseMass;
		float density; // moment of inertia = mass^2 / density
		Vector2f accumForce;
		float accumTorque;
	};
}

