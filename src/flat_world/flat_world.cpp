#include "flat_world.h"
#include "polygon.h"
#include <cmath>

namespace FlatWorld
{
	//Attributes::Attributes()
	//{
	//	velocity = Vector2f::zero();
	//	angularVelocity = 0.0f;
	//	mass = 1.0f;
	//	density = 1.0f;
	//	immovable = false;
	//}

	FlatWorld::FlatWorld(std::shared_ptr<Scene> scene)
	{
		marker = std::make_shared<Polygon>(scene);
	}

	void FlatWorld::iteratePhysics(float deltaTime)
	{
		// Clear accumulated forces
		for (auto entity : entities)
		{
			entity->clearForces();
		}

		// Accumulate forces (user function)
		if(generateForces)
		{
			generateForces();
		}

		// Integrate velocity and position
		for (auto entity : entities)
		{
			Vector2f acceleration = entity->getAccumForce() * entity->getInverseMass();
			float angularAcceleration = entity->getAccumTorque() * entity->getInverseMass() * entity->getInverseMass() * entity->getDensity();

			entity->setVelocity(entity->getVelocity() + acceleration * deltaTime);
			entity->setAngularVelocity(entity->getAngularVelocity() + angularAcceleration * deltaTime);

			entity->setPosition(entity->getPosition() + entity->getVelocity() * deltaTime);
			entity->setOrientation(entity->getOrientation() + entity->getAngularVelocity() * deltaTime);
		}

		// Generate collisions for all entities

		// For each entity, use the collision with the min toi.

		// TESTING
		std::vector<Polygon::Vertex> vertices;
		vertices.push_back(Polygon::Vertex(Vector2f(0, 0)));
		vertices.push_back(Polygon::Vertex(Vector2f(1, 0)));
		vertices.push_back(Polygon::Vertex(Vector2f(0, 1)));
		marker->setVertices(vertices);
	}

	//void FlatWorld::simpleTick(float deltaTime)
	//{


		// Populate impulses from collisions/contacts/constraints

		// Adjust position and velocity from impulses








		// If a collision is found, it just puts it back to its original position

		// You still need TOI to find the impulse direction

		// Move the entities.
		//for(auto & it : entityAttributes)
		//{
		//	auto & entity = it.first;
		//	auto & attributes = it.second;
		//	attributes.tryPositionOffset = attributes.velocity * deltaTime;
		//	attributes.tryRotationOffset = attributes.angularVelocity * deltaTime;
		//	entity->setPosition(entity->getPosition() + attributes.tryPositionOffset);
		//	entity->setOrientation(entity->getOrientation() + attributes.tryRotationOffset);
		//}

		// Handle collisions.
		//for(auto & it1 : entityAttributes)
		//{
		//	auto & entity1 = it1.first;
		//	auto & attributes1 = it1.second;
		//	for(auto & it2 : entityAttributes)
		//	{
		//		auto & entity2 = it2.first;
		//		auto & attributes2 = it2.second;
		//		if(entity1 >= entity2)
		//		{
		//			continue; // only consider any pair only once
		//		}
		//		//Intersection intersection = getIntersection(entity1, entity2);
		//		//if(intersection.yes)
		//		//{
		//		//}
		//	}
		//}

	//}

	//void FlatWorld::tick(float deltaTime)
	//{
		// Move the entities.
		//for(auto & it : entityAttributes)
		//{
		//	auto & entity = it.first;
		//	auto & attributes = it.second;
		//	attributes.tryPositionOffset = attributes.velocity * deltaTime;
		//	attributes.tryRotationOffset = attributes.angularVelocity * deltaTime;
		//	attributes.earliestCollisionTime = 1.0f;
		//	entity->setPosition(entity->getPosition() + attributes.tryPositionOffset);
		//	entity->setOrientation(entity->getOrientation() + attributes.tryRotationOffset);
		//}

		// Handle collisions.
		//for(auto & it1 : entityAttributes)
		//{
		//	auto & entity1 = it1.first;
		//	auto & attributes1 = it1.second;
		//	for(auto & it2 : entityAttributes)
		//	{
		//		auto & entity2 = it2.first;
		//		auto & attributes2 = it2.second;
		//		if(entity1 >= entity2)
		//		{
		//			continue; // only consider any pair only once
		//		}
		//		//Intersection intersection = getIntersection(entity1, entity2);
		//	//	if(intersection.yes)
		//	//	{
		//	//		float massFactor = 0; // 0 means only entity1 moves, 1 means only entity2 moves
		//	//		if(!attributes1.immovable && !attributes2.immovable && (attributes1.mass + attributes2.mass) > 0)
		//	//		{
		//	//			massFactor = attributes1.mass / (attributes1.mass + attributes2.mass);
		//	//		}
		//	//		else if(!attributes1.immovable)
		//	//		{
		//	//			massFactor = 0;
		//	//		}
		//	//		else if(!attributes2.immovable)
		//	//		{
		//	//			massFactor = 1;
		//	//		}
		//	//		if(!attributes1.immovable && !attributes1.tryPositionOffset.isZero())
		//	//		{
		//	//			float collisionTime = attributes1.tryPositionOffset.dot(-intersection.ray.direction * (1.0f - massFactor)) / attributes1.tryPositionOffset.normSq();
		//	//			if(attributes1.earliestCollisionTime > collisionTime)
		//	//			{
		//	//				attributes1.earliestCollisionTime = collisionTime;
		//	//				attributes1.earliestCollisionImpulse = intersection.ray.scaled(massFactor - 1.0f);
		//	//			}
		//	//		}
		//	//		else
		//	//		{
		//	//			attributes1.earliestCollisionTime = 0.0f;
		//	//			attributes1.earliestCollisionImpulse = intersection.ray.scaled(massFactor - 1.0f);
		//	//		}
		//	//		if(!attributes2.immovable && !attributes2.tryPositionOffset.isZero())
		//	//		{
		//	//			float collisionTime = attributes2.tryPositionOffset.dot(intersection.ray.direction * massFactor) / attributes2.tryPositionOffset.normSq();
		//	//			if(attributes2.earliestCollisionTime > collisionTime)
		//	//			{
		//	//				attributes2.earliestCollisionTime = collisionTime;
		//	//				attributes2.earliestCollisionImpulse = intersection.ray.scaled(massFactor);
		//	//			}
		//	//		}
		//	//		else
		//	//		{
		//	//			attributes2.earliestCollisionTime = 0.0f;
		//	//			attributes2.earliestCollisionImpulse = intersection.ray.scaled(massFactor);
		//	//		}
		//	//		attributes1.earliestCollisionTime = 0.0f;
		//	//		attributes2.earliestCollisionTime = 0.0f;
		//	//	}
		//	}
		//}

		//// Move the entities after collisions.
		//for(auto & it : entityAttributes)
		//{
		//	auto & entity = it.first;
		//	auto & attributes = it.second;
		//	if(!attributes.immovable && attributes.earliestCollisionTime < 1.0f)
		//	{
		//		applyImpulse(entity, attributes.earliestCollisionImpulse);
		//		if(attributes.earliestCollisionImpulse.direction.dot(attributes.velocity) < 0)
		//		{
		//			entity->setPosition(entity->getPosition() + attributes.earliestCollisionImpulse.direction);
		//		}
		//	}
		//}
	//}

	void FlatWorld::addEntity(std::shared_ptr<Entity> entity)
	{
		entities.insert(entity);
	}

	void FlatWorld::removeEntity(std::shared_ptr<Entity> entity)
	{
		entities.erase(entity);
	}

	//Intersection FlatWorld::getIntersection(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2)
	//{
	//	return ::FlatWorld::getIntersection(std::dynamic_pointer_cast<Polygon>(entity1), std::dynamic_pointer_cast<Polygon>(entity2));
	//}

	void FlatWorld::applyImpulse(std::shared_ptr<Entity> entity, Ray2f ray)
	{
		//Attributes & attributes = entityAttributes.find(entity)->second;
		//Vector2f r = ray.start - entity->getPosition();
		//if(r.normSq() > 0 && attributes.mass > 0 && attributes.density > 0)
		//{
		//	attributes.velocity += ray.direction / attributes.mass;
		//	attributes.angularVelocity += r.cross2d(ray.direction) * attributes.density / (attributes.mass * attributes.mass);
		//}
	}

	Collision getCollision(std::shared_ptr<Polygon> polygon1, Vector2f const & offset1, std::shared_ptr<Polygon> polygon2, Vector2f const & offset2)
	{
		// Use SAT, but include offsets to calculate a collision.toi. (always >= 0)
		// Just project the offsets on each axis to get a moving range, and then intersect the moving ranges.
		// We're not worrying about rotation offsets, because it's too complicated, and probably won't change things much.
		// Use the minimum > 0 toi.
		// If all axes have a 0 toi (they were intersecting at the start), use the axis with the min overlap.
		return Collision();
	}

	// This uses the SAT to get the intersection ray. The length of the ray is the amount needed to remove the collision.
	//Intersection getIntersection(std::shared_ptr<Polygon> polygon1, std::shared_ptr<Polygon> polygon2)
	//{
	//	/*
	//	// SAT
	//	foreach n of this and other edges
	//		if projection of this and other onto n does not intersect
	//			returun false
	//		(nMin, nMin overlap) = min(n overlap, nMin overlap)
	//	The nMin edge is the contact edge, and the least/greatest vertex on the other polygon is the contact vertex (and ray origin).
	//	The normal is the nMin edge normal.
	//	*/

	//	Intersection intersection;
	//	intersection.yes = false;

	//	std::shared_ptr<Polygon> polygon [2] = {polygon1, polygon2};

	//	float minOverlap = 0;
	//	float min [2];
	//	unsigned int minI [2];
	//	float max [2];
	//	unsigned int maxI [2];

	//	for(int p = 0; p < 2; p++) // for each polygon
	//	{
	//		Ray2f ray;

	//		ray.start = polygon[p]->getPosition();
	//		for(unsigned int i = 0; i < polygon[p]->getVertices().size(); i++) // for each edge in that polygon
	//		{
	//			ray.direction = (polygon[p]->getWorldPositionOfVertex(i) - polygon[p]->getWorldPositionOfVertex((i + 1) % polygon[p]->getVertices().size())).perp2d();
	//			if(p == 1)
	//			{
	//				ray.direction *= -1.0f;
	//			}
	//			polygon[p]->project(ray, min[p], minI[p], max[p], maxI[p]);
	//			polygon[1 - p]->project(ray, min[1 - p], minI[1 - p], max[1 - p], maxI[1 - p]);
	//			if(max[p] > min[1 - p] && max[1 - p] > min[p])
	//			{
	//				// find overlap
	//				float overlap = std::min(max[p] - min[1 - p], max[1 - p] - min[p]);
	//				if(overlap < minOverlap || minOverlap == 0)
	//				{
	//					minOverlap = overlap;
	//					intersection.ray.direction = ray.direction.unit() * minOverlap;
	//					if(max[p] + min[p] <= max[1 - p] + min[1 - p])
	//					{
	//						intersection.ray.start = polygon[1 - p]->getWorldPositionOfVertex(minI[1 - p]);
	//					}
	//					else
	//					{
	//						intersection.ray.start = polygon[1 - p]->getWorldPositionOfVertex(maxI[1 - p]);
	//					}
	//				}
	//			}
	//			else // no intersection, we're done
	//			{
	//				return intersection;
	//			}
	//		}
	//	}

	//	intersection.yes = true;
	//	return intersection;
	//}

	// Gets time and point of impact of moving ranges. pointAtImpact is only valid if timeAtImpact is exclusively between 0 and 1.
	void getCollisionOfRanges(Rangef range1, float offset1, Rangef range2, float offset2, float & timeAtImpact, float & pointAtImpact)
	{
		if(range1.max < range2.min)
		{
			if(offset1 > offset2)
			{
				timeAtImpact = (range2.min - range1.max) / (offset1 - offset2);
				pointAtImpact = range1.max + offset1 * timeAtImpact;
			}
			else
			{
				timeAtImpact = 1;
			}
		}
		else if(range1.min > range2.max && offset2 > offset1)
		{
			if(offset2 > offset1)
			{
				timeAtImpact = (range2.min - range1.max) / (offset1 - offset2);
				pointAtImpact = range2.max + offset2 * timeAtImpact;
			}
			else
			{
				timeAtImpact = 1;
			}
		}
		else
		{
			timeAtImpact = 0;
		}
		if(timeAtImpact > 1)
		{
			timeAtImpact = 1;
		}
	}
}

/*

Collision Notes

When colliding with multiple objects, find out which intersection happens at the earliest offset.
Then apply an impulse to change the objects velocities.
Then, for that intersection, move the objects fully away from each other, so that there are no intersections.


*/