#pragma once

#include "scene_entity.h"

class Light : public SceneEntity
{
public:
	Light();
	Vector3f getColor() const;
	void setColor(Vector3f);

private:
	Vector3f color;
};

