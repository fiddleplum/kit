#pragma once

#include "frame_2d.h"
#include "model_2d.h"

class Camera2D;

class Entity2D
{
public:
	virtual ~Entity2D();

	Vector2f getPosition() const { return frame.getPosition(); }
	virtual void setPosition(Vector2f);

	float getOrientation() const { return frame.getOrientation(); }
	virtual void setOrientation(float);

	int getZ() const { return model.getZ(); }
	virtual void setZ(int);

	Vector2f getScale() const { return model.getScale(); }
	virtual void setScale(Vector2f);

	// Used by Scene2D
	Matrix33f getTransform() const { return frame.getTransform(); }
	bool isSorted() const { return model.isSorted(); }
	void setSorted() { model.setSorted(); }
	class Compare
	{
	public:
		bool operator ()(std::shared_ptr<Entity2D> const & entity0, std::shared_ptr<Entity2D> const & entity1);
	};
	void render(std::shared_ptr<Camera2D> camera);

private:
	Frame2D frame;
	Model2D model;
};

