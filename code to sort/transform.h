#pragma once

#include "Matrix.h"
#include "MathUtil.h"

/*!
A bunch of functions for creating transforms.
In screen coordinates, (0, 0) is the top left and the bottom right is the size of the window.
In normalized device coordinates (NDC), x goes from -1 at the left to +1 on the right, y from -1 at the bottom to +1 at the top, and z from -1 at the near to +1 at the far (left-handed).
In view coordinates, the origin is the position of the camera, the +x-axis goes along the camera's right, the +y-axis goes along the camera's up, and the +z-axis goes along the camera's backward (right-handed). Note that the near and far planes in view coordinates are actually negative.
In world coordinates, the local axes are right, forward, and up for the x, y, and z axes respectively.
*/

namespace Transform
{
  //! Returns a matrix representing a translation of \b v.
  template <typename T> Matrix<4, 4, T> translation (Vector<3, T> v);

  //! Returns a matrix representing a right-handed rotation of \b angle radians about the \b axis.
  template <typename T> Matrix<4, 4, T> rotation (int axis, T angle);

  //! Returns a matrix representing a right-handed rotation about the \b axis, where the norm of \b axis is the angle in radians about which to rotate.
  template <typename T> Matrix<4, 4, T> rotation (Vector<3, T> axis);

  //! Returns a matrix representing a scaling by each element of \b v along the corresponding axis.
  template <typename T> Matrix<4, 4, T> scaling (Vector<3, T> v);

  //! Returns a matrix that transforms from local object coordinates to world coordinates. \b Position is the object's origin, \b right is the object's local +x axis, \b forward the +y axis, and \b up the +z axis.
  template <typename T> Matrix<4, 4, T> localToWorld (Vector<3, T> offset, Vector<3, T> right, Vector<3, T> forward, Vector<3, T> up, Vector<3, T> scale);

  //! Returns a matrix that transforms from world coordinates to local object coordinates. See localToWorld for params.
  template <typename T> Matrix<4, 4, T> worldToLocal (Vector<3, T> position, Vector<3, T> right, Vector<3, T> forward, Vector<3, T> up, Vector<3, T> scale);

  //! Returns a matrix that transforms from world coordinates to view coordinates. In view coordinates, everything is relative to the camera, where the camera's \b position is the origin, the +x axis its \b right, the +y axis its \b up, and the +z axis its \b backward.
  template <typename T> Matrix<4, 4, T> worldToView (Vector<3, T> position, Vector<3, T> right, Vector<3, T> forward, Vector<3, T> up);

  //! Returns a matrix that transforms from view coordinates to world coordinates. See worldToView for params.
  template <typename T> Matrix<4, 4, T> viewToWorld (Vector<3, T> position, Vector<3, T> right, Vector<3, T> forward, Vector<3, T> up);

  //! Returns a matrix that transforms from view coordinates to normalized device coordinates via an orthogonal projection. The \b height is the amount of the camera's y axis to include, and the \b aspect \b ratio (width / height) is of the viewport being rendered. \b Near and \b far are the nearest and farthest points rendered, respectively.
  template <typename T> Matrix<4, 4, T> viewToNDCOrthogonal (T height, T aspectRatio, T near, T far);

  //! Returns a matrix that transforms from normalized device coordinates to view coordinates via an orthogonal projection. See viewToNDCOrthogonal for params.
  template <typename T> Matrix<4, 4, T> ndcToViewOrthogonal (T height, T aspectRatio, T near, T far);

  //! Returns a matrix that transforms from view coordinates to normalized device coordinates via a perspective projection. The \b fovY is the vertical field of view in radians, and the \b aspect \b ratio (width / height) is of the viewport being rendered. \b Near and \b far are the nearest and farthest points rendered, respectively.
  template <typename T> Matrix<4, 4, T> viewToNDCPerspective (T fovY, T aspectRatio, T near, T far);

  //! Returns a matrix that transforms from normalized device coordinates to view coordinates via a perspective projection. See viewToNDCPerspective for params.
  template <typename T> Matrix<4, 4, T> ndcToViewPerspective (T fovY, T aspectRatio, T near, T far);

  //! Returns a matrix that transforms from normalized device coordinates to screen coordinates of the viewport, given \b size.
  template <typename T> Matrix<4, 4, T> ndcToScreen (Vector<2, int> size);

  //! Returns a matrix that transforms from screen coordinates of the viewport, given \b size, to normalized device coordinates.
  template <typename T> Matrix<4, 4, T> screenToNDC (Vector<2, int> size);
};

// Template Implementations

template <typename T>
Matrix<4, 4, T> Transform::translation (Vector<3, T> v)
{
  Matrix<4, 4, T> r = Matrix<4, 4, T>::identity();
  r(0, 3) = v[0];
  r(1, 3) = v[1];
  r(2, 3) = v[2];
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::rotation (int axis, T angle)
{
  assert(0 <= axis && axis < 3);
  Matrix<4, 4, T> r = Matrix<4, 4, T>::identity();
  T sa = (T)sin(angle);
  T ca = (T)cos(angle);
  if (axis == 0)
  {
    r(1, 1) = ca;    r(1, 2) = -sa;
    r(2, 1) = sa;    r(2, 2) = ca;
  }
  else if (axis == 1)
  {
    r(0, 0) = ca;    r(0, 2) = sa;
    r(2, 0) = -sa;   r(2, 2) = ca;
  }
  else if (axis == 2)
  {
    r(0, 0) = ca;    r(0, 1) = -sa;
    r(1, 0) = sa;    r(1, 1) = ca;
  }
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::rotation (Vector<3, T> v)
{
  T angle = v.norm();
  Vector<3, T> vUnit;
  if (angle  != 0)
  {
    vUnit = v.unit();
  }
  else
  {
    vUnit.set(0, 0, 0);
  }
  Matrix<4, 4, T> r;
  T x = vUnit[0], y = vUnit[1], z = vUnit[2];
  T s = (T)sin(angle);
  T c = (T)cos(angle);
  T t = 1 - c;
  if (s == 0)
  {
    return Matrix<4, 4, T>::identity();
  }
  T txy = t * x * y, tyz = t * y * z, tzx = t * z * x, sx = s * x, sy = s * y, sz = s * z;
  r(0, 0) = t * x * x + c; r(0, 1) = txy - sz;      r(0, 2) = tzx + sy;      r(0, 3) = 0;
  r(1, 0) = txy + sz;      r(1, 1) = t * y * y + c; r(1, 2) = tyz - sx;      r(1, 3) = 0;
  r(2, 0) = tzx - sy;      r(2, 1) = tyz + sx;      r(2, 2) = t * z * z + c; r(2, 3) = 0;
  r(3, 0) = 0;             r(3, 1) = 0;             r(3, 2) = 0;             r(3, 3) = 1;
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::scaling (Vector<3, T> v)
{
  Matrix<4, 4, T> r = Matrix<4, 4, T>::identity();
  r(0, 0) = v[0];
  r(1, 1) = v[1];
  r(2, 2) = v[2];
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::localToWorld (Vector<3, T> position, Vector<3, T> right, Vector<3, T> forward, Vector<3, T> up, Vector<3, T> scale)
{
  Matrix<4, 4, T> r;
  T s0 = scale[0], s1 = scale[1], s2 = scale[2];
  r(0, 0) = s0 * right[0]; r(0, 1) = s1 * forward[0]; r(0, 2) = s2 * up[0]; r(0, 3) = position[0];
  r(1, 0) = s0 * right[1]; r(1, 1) = s1 * forward[1]; r(1, 2) = s2 * up[1]; r(1, 3) = position[1];
  r(2, 0) = s0 * right[2]; r(2, 1) = s1 * forward[2]; r(2, 2) = s2 * up[2]; r(2, 3) = position[2];
  r(3, 0) = 0;             r(3, 1) = 0;               r(3, 2) = 0;          r(3, 3) = 1;
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::worldToLocal (Vector<3, T> position, Vector<3, T> right, Vector<3, T> forward, Vector<3, T> up, Vector<3, T> scale)
{
  assert(scale[0] != (T)0 && scale[1] != (T)0 && scale[2] != (T)0);
  Matrix<4, 4, T> r;
  Vector<3, T> rightSInv = right.scaleInv(scale[0]);
  Vector<3, T> forwardSInv = forward.scaleInv(scale[1]);
  Vector<3, T> upSInv = up.scaleInv(scale[2]);
  r(0, 0) = rightSInv[0];   r(0, 1) = rightSInv[1];   r(0, 2) = rightSInv[2];   r(0, 3) = -position.dot(rightSInv);
  r(1, 0) = forwardSInv[0]; r(1, 1) = forwardSInv[1]; r(1, 2) = forwardSInv[2]; r(1, 3) = -position.dot(forwardSInv);
  r(2, 0) = upSInv[0];      r(2, 1) = upSInv[1];      r(2, 2) = upSInv[2];      r(2, 3) = -position.dot(upSInv);
  r(3, 0) = 0;              r(3, 1) = 0;              r(3, 2) = 0;              r(3, 3) = 1;
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::worldToView (Vector<3, T> position, Vector<3, T> right, Vector<3, T> forward, Vector<3, T> up)
{
  return worldToLocal(position, right, up, -forward, Vector<3, T>(1, 1, 1));
}

template <typename T>
Matrix<4, 4, T> Transform::viewToWorld (Vector<3, T> position, Vector<3, T> right, Vector<3, T> forward, Vector<3, T> up)
{
  return localToWorld(position, right, up, -forward, Vector<3, T>(1, 1, 1));
}

template <typename T>
Matrix<4, 4, T> Transform::viewToNDCOrthogonal (T height, T aspectRatio, T near, T far)
{
  assert(height > 0 && aspectRatio > 0 && near < far);
  Matrix<4, 4, T> r = Matrix<4, 4, T>::identity();
  r(0, 0) = (T)2 / (height * aspectRatio);
  r(1, 1) = (T)2 / height;
  r(2, 2) = (T)2 / (near - far);
  r(2, 3) = (near + far) / (near - far);
  r(3, 3) = 1;
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::ndcToViewOrthogonal (T height, T aspectRatio, T near, T far)
{
  assert(height > 0 && aspectRatio > 0 && near < far);
  Matrix<4, 4, T> r = Matrix<4, 4, T>::identity();
  r(0, 0) = (height * aspectRatio) / (T)2;
  r(1, 1) = height / (T)2;
  r(2, 2) = (near - far) / (T)2;
  r(2, 3) = (near + far) / (T)-2;
  r(3, 3) = 1;
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::viewToNDCPerspective (T fovY, T aspectRatio, T near, T far)
{
  assert(0 < fovY && fovY < Math::getPi<T>() && aspectRatio > 0 && near < far);
  Matrix<4, 4, T> r = Matrix<4, 4, T>::zero();
  T tanFovY = tan(fovY / (T)2);
  r(0, 0) = (T)1 / (aspectRatio * tanFovY);
  r(1, 1) = (T)1 / tanFovY;
  r(2, 2) = (near + far) / (near - far);
  r(2, 3) = (T)2 * near * far / (near - far);
  r(3, 2) = -1;
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::ndcToViewPerspective (T fovY, T aspectRatio, T near, T far)
{
  assert(0 < fovY && fovY < Math::getPi<T>() && aspectRatio > 0 && near < far);
  Matrix<4, 4, T> r = Matrix<4, 4, T>::zero();
  T tanFovY = tan(fovY / (T)2);
  r(0, 0) = aspectRatio * tanFovY;
  r(1, 1) = tanFovY;
  r(2, 3) = -1;
  r(3, 2) = (near - far) / ((T)2 * near * far);
  r(3, 3) = (near + far) / ((T)2 * near * far);
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::ndcToScreen (Vector<2, int> size)
{
  assert(size[0] > 0 && size[1] > 0);
  Matrix<4, 4, T> r = Matrix<4, 4, T>::identity();
  r(0, 0) = ((T)size[0] - (T)1) / ((T)2 - (T)2 / (T)size[0]);
  r(1, 1) = ((T)size[1] - (T)1) / ((T)2 / (T)size[1] - (T)2);
  r(0, 3) = ((T)1 - (T)1 / (T)size[0]) * r(0, 0);
  r(1, 3) = ((T)1 / (T)size[1] - (T)1) * r(1, 1);
  return r;
}

template <typename T>
Matrix<4, 4, T> Transform::screenToNDC (Vector<2, int> size)
{
  Matrix<4, 4, T> r = Matrix<4, 4, T>::identity();
  if (size[0] > 1 && size[1] > 1)
  {
    r(0, 0) = ((T)2 - (T)2 / (T)size[0]) / ((T)size[0] - (T)1); // These are more complicated, because an NDC coordinate of +1 is actually one pixel offscreen.
    r(1, 1) = ((T)2 / (T)size[1] - (T)2) / ((T)size[1] - (T)1); // We also need to account for rounding errors, so we effectively map pixel 0 to pixel .5 on-screen.
    r(0, 3) = (T)1 / (T)size[0] - (T)1;
    r(1, 3) = (T)1 - (T)1 / (T)size[1];
  }
  return r;
}

