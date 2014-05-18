#pragma once

#include "../interval.h"
#include <memory>
#include <string>

class Model2D;

namespace Flat
{
	class Sprite
	{
	public:
		Sprite(std::shared_ptr<Model2D> model);
		~Sprite();

		Vector2f getSize() const;
		void setTexture(std::string const & filename);
		void setTextureBounds(Recti bounds);
		Recti getTextureBounds() const;
		void setFrame(int frame);

	private:
		void updateVertices();

		std::shared_ptr<Model2D> model;
		Recti textureBounds;
		int frame;
	};
}

