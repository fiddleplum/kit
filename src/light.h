#pragma once

#include "scene_entity.h"

namespace kit
{
	class Light : public SceneEntity
	{
	public:
		Light();
		Vector3f getColor() const;
		void setColor(Vector3f);

	private:
		Vector3f color;
	};
}

