#include "entity_2d.h"

Entity2D::~Entity2D()
{
}

void Entity2D::setPosition(Vector2f position)
{
	frame.setPosition(position);
}

void Entity2D::setOrientation(float orientation)
{
	frame.setOrientation(orientation);
}

void Entity2D::setZ(int z)
{
	model.setZ(z);
}

void Entity2D::setScale(Vector2f scale)
{
	model.setScale(scale);
}

bool Entity2D::Compare::operator ()(std::shared_ptr<Entity2D> const & entity0, std::shared_ptr<Entity2D> const & entity1)
{
	return entity0->model < entity1->model;
}

void Entity2D::render(std::shared_ptr<Camera2D> camera)
{
	model.render(camera, frame.getTransform());
}

