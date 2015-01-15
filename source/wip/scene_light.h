#pragma once

#include "scene_entity.h"

class SceneLight : public virtual SceneEntity
{
public:
	virtual Coord3f getColor() const = 0;

	virtual void setColor(Coord3f) = 0;
};

