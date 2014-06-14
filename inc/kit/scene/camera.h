#pragma once

#include <kit/scene/entity.h>

namespace kit
{
	namespace scene
	{
		class Camera : virtual public Entity
		{
		public:
			virtual void setAspectRatio (float aspectRatio) = 0;

			virtual void setNear (float near) = 0;

			virtual void setFar (float far) = 0;

			virtual void setPerspective (float fov) = 0;

			virtual void setOrthogonal (float size) = 0;

			virtual void setFor2D (float size) = 0;
		};
	}
}

