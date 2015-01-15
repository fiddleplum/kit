#pragma once

#include "../Coord.h"
#include "../Quaternion.h"
#include "../Matrix.h"

namespace Scene
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity() {}

		Coord2f const & getPosition() const;
		virtual void setPosition(Coord2f);

		float const & getOrientation() const;
		virtual void setOrientation(float);

		Coord2f const & getScale() const;
		virtual void setScale(Coord2f);

		Matrix33f const & getMatrix() const;
		Matrix33f  const & getMatrixInverse() const;

		void updateMatrices();

		Coord2f position;
		float orientation;
		Coord2f scale;
		bool matricesNeedUpdate;
		Matrix33f matrix;
		Matrix33f matrixInverse;
	};
}

