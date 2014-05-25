#pragma once

#include "body.h"
#include "sprite.h"
#include "shape.h"
#include "../frame_2d.h"
#include "../ray.h"
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <set>

class Entity2D;
class Scene2D;

namespace Flat
{
	class Entity
	{
	public:
		Entity(bool frame, bool sprite, int shape, bool body, std::shared_ptr<Scene2D> scene);
		std::shared_ptr<Frame2D> frame;
		std::shared_ptr<Sprite> sprite;
		std::shared_ptr<Shape> shape;
		std::shared_ptr<Body> body;
		std::shared_ptr<Entity2D> entity2D;
	};

	class Impact
	{
	public:
		float time;
		Ray2f ray; // the length of the ray is the interpenetration, the direction goes from object1 to object2
		bool useRayStart1; // if false, only use the direction of the ray, and apply only the linear impulse (no rotation)
		bool useRayStart2;
	};

	class World
	{
	public:
		World();

		void iteratePhysics(float deltaTime);

		void addEntity(std::shared_ptr<Entity>);
		void removeEntity(std::shared_ptr<Entity>);

		void applyImpulse(std::shared_ptr<Entity>, Ray2f);

	private:
		std::function<void ()> generateForces;
		std::set<std::shared_ptr<Entity>> entities;

		std::shared_ptr<Entity> marker;

		std::function<void ()> impactFunctions [Shape::NumTypes][Shape::NumTypes];
	};

	//Collision getCollision(std::shared_ptr<Polygon> polygon1, Vector2f const & offset1, std::shared_ptr<Polygon> polygon2, Vector2f const & offset2);

	//Intersection getIntersection(std::shared_ptr<Polygon> polygon1, std::shared_ptr<Polygon> polygon2);
}

