#pragma once

#include "scene_entity.h"

class SceneLight : public virtual SceneEntity
{
public:
	virtual Vector3f getColor() const = 0;

	virtual void setColor(Vector3f) = 0;
};

