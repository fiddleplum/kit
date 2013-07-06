#pragma once

#include "Vector.h"
#include "Matrix.h"

class Object3D
{
public:
  Object3D ();

  Vector<3, float> position;

  // The orthonormal trio of vectors that describe the orientation of the object. They are right, forward, and up, respectively.
  Vector<3, Vector<3, float>> basis;

  // The matrix that transforms local into world coordinates.
  Matrix<4, 4, float> world_matrix () const;

  // The length of the axis is the angle in radians to rotate.
  void rotate (Vector<3, float> const& axis);

  // The make_level will force basis[0][2] to be zero.
  void look_at (Vector<3, float> const& target, bool make_level);

  void orbit (float distance, float yaw, float pitch, float roll, Vector<3, float> const& target);
};

