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

			virtual void setPosition (Vector3f position) = 0;

			virtual Quaternionf const & getOrientation () const = 0;

			virtual void setOrientation (Quaternionf orientation) = 0;
		};
	}
}

