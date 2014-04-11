#pragma once

#include "Entity.h"
#include "../Interval.h"
#include "../Ray.h"
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <set>


#include "../Scene.h"
namespace FlatWorld
{
	class Polygon;

	class Collision
	{
	public:
		float timeOfImpact;
		Ray2f ray; // the length of the ray is the interpenetration, the direction goes from object1 to object2
		bool useRayStart1; // if false, only use the direction of the ray, and apply only the linear impulse (no rotation)
		bool useRayStart2;
	};

	//class Attributes
	//{
	//public:
	//	Attributes();

	//	Vector2f tryPositionOffset;
	//	float tryRotationOffset;
	//	float earliestCollisionTime; // (0 -> 1) * deltaTime
	//	Vector2f earliestCollisionOffset;
	//	Ray2f earliestCollisionImpulse;
	//	Vector2f velocity;
	//	float angularVelocity;
	//	float mass;
	//	float density;
	//	bool immovable;
	//};

	class FlatWorld
	{
	public:
		FlatWorld(std::shared_ptr<Scene> scene);

		void iteratePhysics(float deltaTime);

		void addEntity(std::shared_ptr<Entity>);
		void removeEntity(std::shared_ptr<Entity>);

		//Intersection getIntersection(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2);

		void applyImpulse(std::shared_ptr<Entity>, Ray2f);

	private:
		std::function<void ()> generateForces;
		std::set<std::shared_ptr<Entity>> entities;

		std::shared_ptr<Polygon> marker;
	};

	Collision getCollision(std::shared_ptr<Polygon> polygon1, Vector2f const & offset1, std::shared_ptr<Polygon> polygon2, Vector2f const & offset2);

	//Intersection getIntersection(std::shared_ptr<Polygon> polygon1, std::shared_ptr<Polygon> polygon2);
}

