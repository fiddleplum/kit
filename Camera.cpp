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

Matrix<4, 4, float> Camera::projection_matrix() const
{
  return ::perspective_matrix<float>(focal_length, aspect_ratio, near_z, far_z);
}

Matrix<4, 4, float> Camera::view_matrix() const
{
  return ::view_matrix<float>(position, basis(0), basis(1));
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

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(projection_matrix().c);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(view_matrix().c);
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
}

