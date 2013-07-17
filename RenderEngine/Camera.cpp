#include "Camera.h"
#include <exception>

Camera::Camera()
{
	aspectRatio = 1.0f;
	near = 0.1f;
	far = 1000.0f;
	fov = 1.0f;
	size = 1.0f;
	perspective = true;
	projection = Matrix44f::zero();
	view = Matrix44f::identity();
}

Framef const & Camera::getFrame() const
{
	return frame;
}

Framef & Camera::getFrame()
{
	viewNeedsUpdate = true;
	return frame;
}

Ray<3, float> Camera::get_ray(Vector<2, int> const& position_in_window,
    Vector<2, int> const& window_size)
{
  Ray<3, float> ray;
  ray.start = position;
  ray.direction = unit(transform(world_matrix(), Vector<3, float>(
     (((float) position_in_window(0) /(window_size(0) / 2)) - 1.0f),
      focal_length,(((float) position_in_window(1) / -(window_size(1) / 2))
          + 1.0f)), 0.0f));
  return ray;
}

void Camera::activate()
{
	glViewport(viewport.min[0], viewport.min[1], viewport.max[0] - viewport.min[0], viewport.max[1] - viewport.min[1]);
}

Matrix44f const & Camera::getProjection() const
{
	if(projectionNeedsUpdate)
	{
		const_cast<Camera *>(this)->updateProjection();
	}
	return projection;
}

Matrix44f const & Camera::getView() const
{
	if(viewNeedsUpdate)
	{
		const_cast<Camera *>(this)->updateView();
	}
	return view;
}

void Camera::updateProjection()
{
	float scale;
	if(perspective)
	{
		scale = std::tan(fov / 2.0f);
	}
	else
	{
		scale = size;
	}
	if(scale == 0)
	{
		throw std::exception();
	}
	float scaleInv = 1.0f / scale;
	if(aspectRatio >= 1.0f)
	{
		projection(0, 0) = scaleInv / aspectRatio;
		projection(1, 1) = scaleInv;
	}
	else
	{
		projection(0, 0) = scaleInv;
		projection(1, 1) = scaleInv * aspectRatio;
	}
	if(perspective)
	{
		projection(2, 2) = (near + far) / (near - far);
		projection(2, 3) = 2.0f * near * far / (near - far);
		projection(3, 2) = -1.0f;
		projection(3, 3) = 0.0f;
	}
	else
	{
		projection(2, 2) = 
		projection(3, 2) = 0.0f;
		projection(3, 3) = 1.0f;
	}
}

void Camera::updateView()
{
	Vector3f position = frame.getPosition();
	Matrix<3, 3, T> rot = frame.getOrientation().getMatrix();
	view(0, 0) = rot(0, 0);
	view(1, 0) = rot(0, 1);
	view(2, 0) = rot(0, 2);
	view(0, 1) = rot(2, 0);
	view(1, 1) = rot(2, 1);
	view(2, 1) = rot(2, 2);
	view(0, 2) = -rot(1, 0);
	view(1, 2) = -rot(1, 1);
	view(2, 2) = -rot(1, 2);
	view(0, 3) = (-position[0] * rot(0, 0) - position[1] * rot(2, 0) + position[2] * rot(1, 0));
	view(1, 3) = (-position[0] * rot(0, 1) - position[1] * rot(2, 1) + position[2] * rot(1, 1));
	view(2, 3) = (-position[0] * rot(0, 2) - position[1] * rot(2, 2) + position[2] * rot(1, 2));
}

