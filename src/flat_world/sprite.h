#pragma once

#include "../interval.h"
#include <memory>
#include <string>

class Entity2D;

namespace FlatWorld
{
	class Sprite
	{
	public:
		Sprite();
		virtual ~Sprite();

		Rectf getBounds() const;
		void setPosition(Vector2f position);

		void setTexture(std::string const & filename);
		void setTextureBounds(Recti bounds);
		Recti getTextureBounds() const;
		void setFrame(int frame);

	private:
		void updateVertices();

		std::shared_ptr<Entity2D> entity;
		Recti textureBounds;
		int frame;
	};
}

