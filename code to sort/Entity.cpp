#include "Entity.h"
#include <SFML/Window/OpenGL.hpp>

Entity::Entity ()
{
  object3d = new Object3D();
}

Entity::~Entity ()
{
  delete object3d;
}

Object3D * Entity::object ()
{
  return object3d;
}

void Entity::setMesh (Mesh * mesh_)
{
  mesh = mesh_;
}

void Entity::setTexture (char const* filename)
{
  texture.LoadFromFile(filename);
  glGenTextures(1, (GLuint *) &textureId);
  glBindTexture(GL_TEXTURE_2D, (GLuint) textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.GetWidth(),
      texture.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.GetPixelsPtr());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
}
void Entity::setCurrentFrame (int currentFrame_)
{
  currentFrame = currentFrame_;
}
void Entity::draw ()
{
  Matrix44f matrix = object3d->world_matrix();
  glPushMatrix();
  glMultMatrixf(matrix.c);
  glBindTexture(GL_TEXTURE_2D, (GLuint) textureId);
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < (int) mesh->triangles.size(); ++i)
  {
    Mesh::Vertex vertex[3];
    Mesh::TexCoord texCoord[3];
    vertex[0]
        = mesh->frames[currentFrame].vertices[mesh->triangles[i].vertices[0]];
    vertex[1]
        = mesh->frames[currentFrame].vertices[mesh->triangles[i].vertices[1]];
    vertex[2]
        = mesh->frames[currentFrame].vertices[mesh->triangles[i].vertices[2]];
    texCoord[0] = mesh->texCoords[mesh->triangles[i].texCoords[0]];
    texCoord[1] = mesh->texCoords[mesh->triangles[i].texCoords[1]];
    texCoord[2] = mesh->texCoords[mesh->triangles[i].texCoords[2]];
    glTexCoord2fv(texCoord[0].c);
    glNormal3fv(vertex[0].n.c);
    glVertex3fv(vertex[0].p.c);
    glTexCoord2fv(texCoord[1].c);
    glNormal3fv(vertex[1].n.c);
    glVertex3fv(vertex[1].p.c);
    glTexCoord2fv(texCoord[2].c);
    glNormal3fv(vertex[2].n.c);
    glVertex3fv(vertex[2].p.c);
  }
  glEnd();
  glPopMatrix();
}
