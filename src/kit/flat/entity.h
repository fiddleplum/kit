#pragma once

#include "../vector.h"
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

			Vector2f getPosition() const { return _position; }

			virtual void setPosition(Vector2f position);

			float getOrientation() const { return _orientation; }

			virtual void setOrientation(float orientation);

			int getZ() const { return _z; }

			virtual void setZ(int z);

			Matrix33f const & getTransform() const;

			Matrix33f const & getTransformInv() const;

		private:
			void updateTransforms();

			Vector2f _position;
			float _orientation;
			int _z;
			Matrix33f _transform;
			Matrix33f _transformInv;
			bool _transformsDirty;
		};
	}
}

