#pragma once

#include "scene_entity.h"

class SceneLight : public SceneEntity
{
public:
	SceneLight();

	Coord3f getColor() const;

	void setColor(Coord3f);

private:
	Coord3f color;
};

