#pragma once

#include "entity.h"

namespace kit
{
	namespace scene
	{
		class Light : public Entity
		{
		public:
			Light();

			Vector3f getColor() const;

			void setColor(Vector3f);

		private:
			Vector3f color;
		};
	}
}
