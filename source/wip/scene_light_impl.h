#pragma once

#include "scene_entity_impl.h"
#include <kit/scene_light.h>

class SceneLightImpl : public virtual SceneEntityImpl, public virtual SceneLight
{
public:
	SceneLightImpl();

	Coord3f getColor() const override;

	void setColor(Coord3f) override;

private:
	Coord3f color;
};

