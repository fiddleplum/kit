#pragma once

#include "Frame.h"
#include "Ray.h"

class Camera
{
public:
	Camera();

	void setAspectRatio(float aspectRatio);

	void setNearFar(float near, float far);

	void setPerspective(float fov);

	void setOrthogonal(float size);

	Vector3f const & getPosition() const;

	void setPosition(Vector3f const & position);

	Quaternionf const & getOrientation() const;

	void setOrientation(Quaternionf const & orientation);

	Ray3f getRay(Vector2f const& ndcPosition) const;

	Matrix44f getProjection() const;

	Matrix44f getView() const;

private:
	void updateProjection();

	void updateView();

	float aspectRatio;
	float near;
	float far;
	float fov;
	float size;
	bool perspective;
	Frame frame;
	Matrix44f projection;
	Matrix44f view;
};

