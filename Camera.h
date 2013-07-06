#pragma once

#include "Object3D.h"
#include "Ray.h"

class Camera : public Object3D {
public:
	Camera();

	float focal_length;
	float near_z;
	float far_z;
	Matrix<4,4,float> projection_matrix() const;
	Matrix<4,4,float> view_matrix() const;

	Ray<3, float> get_ray(Vector<2, int> const& position_in_window, Vector<2, int> const& window_size);

	void activate();

private:
	float aspect_ratio;
};

