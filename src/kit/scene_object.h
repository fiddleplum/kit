#pragma once

#include "scene_entity.h"
#include "scene_model.h"
#include "ptr.h"
#include <string>

class SceneObject : public SceneEntity
{
public:
	float getScale() const;

	void setScale(float scale);

	Ptr<SceneModel> getModel() const;

	void setModel(Ptr<SceneModel> model);

	void setModel(std::string const & filename);

private:
	Ptr<SceneModel> model;
};

