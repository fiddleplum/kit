#pragma once

#include "Entity.h"
#include "../Interval.h"
#include "../Ray.h"
#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace FlatWorld
{
	class Polygon;

	class Collision
	{
	public:
		float timeOfImpact;
		Ray2f ray;
	};

	class Attributes
	{
	public:
		Attributes();

		Vector2f tryPositionOffset;
		float tryRotationOffset;
		float earliestCollisionTime; // (0 -> 1) * deltaTime
		Vector2f earliestCollisionOffset;
		Ray2f earliestCollisionImpulse;
		Vector2f velocity;
		float angularVelocity;
		float mass;
		float density;
		bool immovable;
	};

	class FlatWorld
	{
	public:
		void simpleTick(float deltaTime);
		void tick(float deltaTime);

		void addEntity(std::shared_ptr<Entity>, Attributes const &);
		void removeEntity(std::shared_ptr<Entity>);
		Attributes & getAttributes(std::shared_ptr<Entity>);

		//Intersection getIntersection(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2);

		void applyImpulse(std::shared_ptr<Entity>, Ray2f);

	private:
		std::function<void ()> generateForces;
		std::vector<std::shared_ptr<Entity>> entities;
		std::map<std::shared_ptr<Entity>, Attributes> entityAttributes;
	};

	Collision getCollision(std::shared_ptr<Polygon> polygon1, Vector2f const & offset1, std::shared_ptr<Polygon> polygon2, Vector2f const & offset2);

	//Intersection getIntersection(std::shared_ptr<Polygon> polygon1, std::shared_ptr<Polygon> polygon2);
}

