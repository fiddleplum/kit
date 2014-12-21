#include "Object3D.h"
#include "Transform.h"

Object3D::Object3D ()
{
  position = zero_vector<3, float> ();
  basis(0) = axis_vector<3, float> (0);
  basis(1) = axis_vector<3, float> (1);
  basis(2) = axis_vector<3, float> (2);
}

Matrix<4, 4, float> Object3D::world_matrix () const
{
  return ::world_matrix<float>(position, basis(0), basis(1), filled_vector<3,
      float> (1.0f));
}

void Object3D::rotate (Vector<3, float> const& axis)
{
  float angle = norm(axis);
  Matrix<4, 4, float> t = rotation_matrix<float> (axis * (1 / angle), angle);
  basis(0) = transform(t, basis(0), 0.0f);
  basis(1) = transform(t, basis(1), 0.0f);
  basis(2) = transform(t, basis(2), 0.0f);
}

void Object3D::look_at (Vector<3, float> const& target, bool make_level)
{
  Vector<3, float> current_trace = target - position;
  if (current_trace == zero_vector<3, float> ()) // on top of target, so any direction is valid
    return;
  basis(1) = unit(current_trace);
  if (make_level)
  { // make the view level with the horizontal
    if (basis(1)(2) != 1) // if not looking straight up
      basis(0) = unit(cross(basis(1), axis_vector<3, float> (2)));
    else if (basis(0)(2) != 1) // if previous right was not straight up
      basis(0) = unit(Vector<3, float> (basis(0)(0), basis(0)(1), 0.0f));
    else
      // just use a default vector
      basis(0) = axis_vector<3, float> (0);
    basis(2) = cross(basis(0), basis(1));
  }
  else
  {
    if (1 - dot(basis(1), basis(0)) >= .01f) // if forward and previous right don't coincide change up, otherwise up doesn't need to change up
      basis(2) = unit(cross(basis(0), basis(1)));
    basis(1) = cross(basis(2), basis(0)); // align right
  }
}

void Object3D::orbit (float distance, float yaw, float pitch, float roll,
    Vector<3, float> const& target)
{
  if (pitch > 3.14159f / 2.0f)
    pitch = 3.14159f / 2.0f;
  if (pitch < -3.14159f / 2.0f)
    pitch = -3.14159f / 2.0f;
  // yaw, pitch, roll; alpha, beta, gamma
  float cy = cos(yaw), sy = sin(yaw), cp = cos(pitch), sp = sin(pitch), cr =
      cos(roll), sr = sin(roll);
  basis(0)(1) = -cy * sp * sr + sy * cr;
  basis(0)(0) = -sy * sp * sr - cy * cr;
  basis(0)(2) = cp * sr;
  basis(1)(1) = -cp * cy;
  basis(1)(0) = -cp * sy;
  basis(1)(2) = -sp;
  basis(2)(1) = cy * sp * cr + sy * sr;
  basis(2)(0) = sy * sp * cr - cy * sr;
  basis(2)(2) = -cp * cr;
  position = target - (distance * basis(1));
}
