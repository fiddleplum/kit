#pragma once

#include "box.hpp"
#include <cstdio>

// This is a set of intersection functions

template <class T>
T abs (T a)
{
  return a > 0 ? a : -a;
}

// u is the fraction of the distance from original_center to center
template <class T>
bool intersect_axis_unit_square_box (T & u, Vector<3, T> & normal, int axis, Vector<3, T> center_u0, Vector<3, T> center_u1, Vector<3, T> half_size)
{
  float u_d;
  Vector<3, T> r = center_u1 - center_u0;

  printf("axis = %d, r = (%f, %f, %f)\n", axis, r[0], r[1], r[2]);
  printf("center_u0 = (%f, %f, %f), center_u1 = (%f, %f, %f)\n", center_u0[0], center_u0[1], center_u0[2], center_u1[0], center_u1[1], center_u1[2]);

  // Get distance needed to travel on any axis to remove overlap
  bool u_set = false;
  normal.set(0, 0, 0);
  int max_d = 0;
  float sink_depth = (T)0.1;
  bool starting_inside = true;
  for (int d = 0; d < 3; ++d)
  {
    if (r[d] > 0)
    {
      if (center_u1[d] + half_size[d] <= 0 || (d == axis ? 0 : 1) <= center_u0[d] - half_size[d])
        return false;
      if (sink_depth > center_u0[d] + half_size[d])
        starting_inside = false;
      u_d = (T)1 - (center_u1[d] + half_size[d]) / r[d];
      printf("d = %d, u_d = %f\n", d, u_d);
    }
    else if (r[d] < 0)
    {
      if (center_u0[d] - half_size[d] <= (d == axis ? 0 : 1) - sink_depth || (d == axis ? 0 : 1) <= center_u1[d] - half_size[d])
        return false;
      if (center_u0[d] - half_size[d] > (d == axis ? 0 : 1) - sink_depth)
        starting_inside = false;
      u_d = (T)1 - (center_u1[d] - half_size[d] - (d == axis ? 0 : 1)) / r[d];
      printf("d = %d, u_d = %f\n", d, u_d);
    }
    else
    {
      if (center_u0[d] + half_size[d] <= 0 || (d == axis ? 0 : 1) <= center_u0[d] - half_size[d])
        return false;
      continue;
    }
    if (!u_set || u <= u_d)
    {
      u = u_d;
      normal.set(0, 0, 0);
      normal[d] = r[d] > 0 ? -1 : +1;
      max_d = d;
      u_set = true;
    }
  }
  if (starting_inside)
    return false;
  if (!u_set) // |r| = 0
  {
    u = 0;
    normal.set(0, 0, 0);
  }

  printf("max_d = %d, u = %f, r[max_d] = %f, half_size = (%f, %f, %f)\n", max_d, u, r[max_d], half_size[0], half_size[1], half_size[2]);

  return true;
}

#if 0
// u is the fraction of the distance from original_center to center
template <class T>
bool intersect_box_box (T & u, Vector<3, T> & normal, Box<3, T> box0, Box<3, T> box1, Vector<3, T> original_center)
{
  // check if they overlap
  if (box.max[axis] <= 0 || 0 <= box.min[axis] || box.max[axis_1] <= 0 || 1 <= box.min[axis_1] || box.max[axis_2] <= 0 || 1 <= box.min[axis_2])
    return false; // the box is outside of the plane

  Vector<3, T> offset(0, 0, 0);
  Vector<3, T> box_center = box.center();
  Vector<3, T> r = box_center - original_center;

  // Get distance needed to travel on any axis to remove overlap
  for (int d = 0; d < 3; ++d)
  {
    if (r[d] > 0)
      offset[d] = -box.max[d];
    else if (r[d] < 0)
    {
      offset[d] = -box.min[d];
      if (d != axis)
        offset[d] += (T)1.0;
    }
  }

  // Get the maximum u for the collision
  u = 1;
  normal.set(0, 0, 0);
  bool moved = false;
  for (int d = 0; d < 3; ++d)
  {
    if (r[d] == 0)
      continue;
    float u_d = (T)1 - (-offset[d] / r[d]);
    if (!moved || u < u_d)
    {
      normal.set(0, 0, 0);
      normal[d] = r[d] > 0 ? -1 : +1;
      u = u_d;
      moved = true;
    }
  }

//  if (u < 0)
//    return false; // box was already inside face

  return true;
}
#endif

template <class T>
bool intersect (Ray<2, T> & ray, Box<2, T> box0, Box<2, T> box1)
{
  Box<2, T> overlap;
  for (int i = 0; i < 2; ++i)
  {
    if (box1.min[i] < box0.min[i])
      overlap.min[i] = box0.min[i];
    else
      overlap.min[i] = box1.min[i];
    if (box1.max[i] > box0.max[i])
      overlap.max[i] = box0.max[i];
    else
      overlap.max[i] = box1.max[i];
  }
  Vector<2, T> d = overlap.max - overlap.min;
  if (d[0] > 0 && d[1] > 0)
  {
    ray.start = overlap.center();
    if (d[1] > d[0])
    {
      if (box1.center()[0] > box0.center()[0])
        ray.direction.set(d[0] * (T)+0.5, 0);
      else
        ray.direction.set(d[0] * (T)-0.5, 0);
    }
    else
    {
      if (box1.center()[1] > box0.center()[1])
        ray.direction.set(0, d[1] * (T)+0.5);
      else
        ray.direction.set(0, d[1] * (T)-0.5);
    }
    return true;
  }
  return false;
}