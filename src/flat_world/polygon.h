#pragma once

#include "entity.h"
#include "../vector.h"
#include "../interval.h"
#include "../ray.h"
#include "../model.h"
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
			Vertex(Vector2f p, Vector2f uv) { this->p = p; this->uv = uv; }
			Vector2f p;
			Vector2f uv;
		};

		Polygon(std::shared_ptr<Scene>);
		~Polygon();

		void setPosition(Vector2f) override;
		void setOrientation(float) override;
		void setScale(Vector2f) override;
		void setZ(int) override;

		std::vector<Vertex> const & getVertices() const { return vertices; }
		void setVertices(std::vector<Vertex>);
		void setRectSprite(Vector2f size, Vector2i uvStart, std::string textureFilename);

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

