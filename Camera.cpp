#include "Camera.h"
#include "Transform.h"
#include <SFML/Window/OpenGL.hpp>

Camera::Camera()
{
  focal_length = 1.0f;
  near_z = 0.1f;
  far_z = 1000.0f;
  aspect_ratio = 1.0f;
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

void Camera::setup_for_rendering()
{
  // set aspect ratio
  int viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  if(viewport[3] != 0)
    aspect_ratio =(float) viewport[2] /(float) viewport[3];

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(projection_matrix().c);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(view_matrix().c);
}

