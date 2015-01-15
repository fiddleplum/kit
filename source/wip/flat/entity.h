#pragma once

#include "../coord.h"
#include "../matrix.h"

namespace kit
{
	namespace flat
	{
		class Entity
		{
		public:
			Entity();

			~Entity() {}

			Coord2f getPosition() const { return _position; }

			virtual void setPosition(Coord2f position);

			float getOrientation() const { return _orientation; }

			virtual void setOrientation(float orientation);

			int getZ() const { return _z; }

			virtual void setZ(int z);

			Matrix33f const & getTransform() const;

			Matrix33f const & getTransformInv() const;

		private:
			void updateTransforms();

			Coord2f _position;
			float _orientation;
			int _z;
			Matrix33f _transform;
			Matrix33f _transformInv;
			bool _transformsDirty;
		};
	}
}

