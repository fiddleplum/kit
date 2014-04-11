#pragma once

#include "Entity.h"
#include "../Vector.h"
#include "../Interval.h"
#include "../Ray.h"
#include "../Model.h"
#include <vector>

class Scene;

namespace FlatWorld
{
	class Polygon : public Entity
	{
	public:
		class Vertex
		{
		public:
			Vertex(Vector2f p) { position = p; }
			Vector2f position;
		};

		Polygon(std::shared_ptr<Scene>);
		~Polygon();

		void setPosition(Vector2f) override;
		void setOrientation(float) override;
		void setScale(Vector2f) override;
		void setZ(int) override;

		std::vector<Vertex> const & getVertices() const { return vertices; }
		void setVertices(std::vector<Vertex>);

		void setColor(Vector3f);
		Rectf getBoundingRect() const;

		bool intersects2(Polygon const & other, Ray2f & intersectionTowardOther) const;
		void project(Ray2f ray, float & min, unsigned int & minI, float & max, unsigned int & maxI) const;

		Vector2f getWorldPositionOfVertex(unsigned int i) const;

	private:
		void updateTransformedBoundingRect();
		void updateModel();

		Rectf boundingRect;
		Rectf worldBoundingRect;

		Vector3f color;

		std::vector<Vertex> vertices;
		std::shared_ptr<Scene> scene;
		std::shared_ptr<Model> model;
	};
}

