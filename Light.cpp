#include "Light.h"
#include <SFML/Window/OpenGL.hpp>
#include <map>

std::map<int, Light *> light_indices;

Light::Light ()
{
  ambient_color = Vector<4, float> (0, 0, 0, 1);
  diffuse_color = specular_color = Vector<4, float> (1, 1, 1, 1);
  position.set(0, 0, 0);
  registered = false;
}

Light::~Light ()
{
  assert(!registered);
}

bool Light::register_with_opengl ()
{
  assert(!registered);
  // set opengl index
  index = 0;
  for (auto i = light_indices.begin(); i != light_indices.end(); ++i)
  {
    if (index == GL_MAX_LIGHTS)
      return false;
    if (i->first != index)
    { // find the first free index and assign it
      light_indices[index] = this;
      break;
    }
    index++;
  }
  registered = true;
  return true;
}

void Light::unregister_from_opengl ()
{
  assert(registered);
  light_indices.erase(index);
  registered = false;
}

void Light::enable_lighting_in_opengl ()
{
  glEnable(GL_LIGHTING);
}

void Light::disable_lighting_in_opengl ()
{
  glDisable(GL_LIGHTING);
}

void Light::setup_for_rendering ()
{
  assert(registered);
  Vector<4, float> position_4(position(0), position(1), position(2), 1.0f);
  glEnable(GL_LIGHT0 + index);
  glLightfv(GL_LIGHT0 + index, GL_POSITION, position_4.c);
  glLightfv(GL_LIGHT0 + index, GL_AMBIENT, ambient_color.c);
  glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, diffuse_color.c);
  glLightfv(GL_LIGHT0 + index, GL_SPECULAR, specular_color.c);
}
