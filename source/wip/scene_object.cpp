#include "scene_object.h"
#include "scene_model.h"
#include "resource_cache.h"

float SceneObject::getScale() const
{
	return model->getScale();
}

void SceneObject::setScale(float scale)
{
	model->setScale(scale);
}

Ptr<SceneModel> SceneObject::getModel() const
{
	return model;
}

void SceneObject::setModel(Ptr<SceneModel> model)
{
	this->model = model;
}

void SceneObject::setModel(std::string const & filename)
{
	model = ResourceCache::getModelFromFile(filename);
}

