#pragma once

#include "Entity.h"

namespace Scene
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

