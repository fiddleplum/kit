#pragma once

#include "scene_entity_impl.h"
#include <kit/scene_light.h>

class SceneLightImpl : public virtual SceneEntityImpl, public virtual SceneLight
{
public:
	SceneLightImpl();

	Vector3f getColor() const override;

	void setColor(Vector3f) override;

private:
	Vector3f color;
};

