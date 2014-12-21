#include "entity.h"

namespace kit
{
	namespace flat
	{
		Entity::Entity()
		{
			_position.set(0.0f, 0.0f);
			_orientation = 0.0f;
			_transformsDirty = true;
			_z = 0;
			_transform = _transformInv = Matrix33f::identity();
		}

		void Entity::setPosition(Vector2f position)
		{
			_position = position;
			_transformsDirty = true;
		}

		void Entity::setOrientation(float orientation)
		{
			_orientation = orientation;
			_transformsDirty = true;
		}

		void Entity::setZ(int z)
		{
			_z = z;
		}

		Matrix33f const & Entity::getTransform() const
		{
			if(_transformsDirty)
			{
				const_cast<Entity *>(this)->updateTransforms();
			}
			return _transform;
		}

		Matrix33f const & Entity::getTransformInv() const
		{
			if(_transformsDirty)
			{
				const_cast<Entity *>(this)->updateTransforms();
			}
			return _transformInv;
		}

		void Entity::updateTransforms()
		{
			Vector2f basis [2];
			basis[0].set(cos(_orientation), sin(_orientation));
			basis[1] = basis[0].perp2d();

			_transform(0, 0) = basis[0][0];
			_transform(1, 0) = basis[0][1];
			_transform(0, 1) = basis[1][0];
			_transform(1, 1) = basis[1][1];
			_transform(0, 2) = _position[0];
			_transform(1, 2) = _position[1];
			_transformInv(0, 0) = basis[0][0];
			_transformInv(1, 0) = basis[1][0];
			_transformInv(0, 1) = basis[0][1];
			_transformInv(1, 1) = basis[1][1];
			_transformInv(0, 2) = -basis[0].dot(_position);
			_transformInv(1, 2) = -basis[1].dot(_position);
			_transformsDirty = false;
		}
	}
}

