#pragma once

#include <kit/vector.h>
#include <kit/quaternion.h>

namespace kit
{
	namespace scene
	{
		class Entity
		{
		public:
			virtual Vector3f const & getPosition () const = 0;

			virtual void setPosition (Vector3f) = 0;

			virtual Quaternionf const & getOrientation () const = 0;

			virtual void setOrientation (Quaternionf) = 0;

			virtual Vector3f const & getScale () const = 0;

			virtual void setScale (Vector3f) = 0;
		};
	}
}

