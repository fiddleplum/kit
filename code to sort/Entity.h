#pragma once

#include "Mesh.h"
#include "Object3D.h"
#include <SFML/Graphics/Image.hpp>

class Entity
{
public:
  Entity ();
  ~Entity ();

  Object3D * object ();
  void setMesh(Mesh * mesh);
  void setTexture (char const* filename);
  void setCurrentFrame (int currentFrame);
  void draw ();

private:
  Object3D * object3d;
  Mesh * mesh;
  int textureId;
  sf::Image texture;
  int currentFrame;
};
