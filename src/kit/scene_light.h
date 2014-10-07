#pragma once

#include "scene_entity.h"

class SceneLight : public SceneEntity
{
public:
	SceneLight();

	Vector3f getColor() const;

	void setColor(Vector3f);

private:
	Vector3f color;
};

