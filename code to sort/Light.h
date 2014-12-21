#pragma once

#include "Vector.h"
#include "Object3D.h"

class Light: public Object3D
{
public:
  Light ();
  ~Light ();

  bool register_with_opengl ();
  void unregister_from_opengl ();

  static void enable_lighting_in_opengl ();
  static void disable_lighting_in_opengl ();
  void setup_for_rendering ();

  float radiance;
  Vector<4, float> ambient_color;
  Vector<4, float> diffuse_color;
  Vector<4, float> specular_color;

private:
  bool registered;
  int index;
};
