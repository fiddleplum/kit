#pragma once

// The model format is at the bottom.

class Model
{
};

/*

Model File Format

Strings are UTF-8 and null terminated.
Ints and floats are 32 bits.
Lists begin with an Int indicating their length and then the elements tightly packed.

model:
mesh list - list of meshes

mesh:
string - name
material - the material
int - num uvs
vertex list - list of vertices
int list - list of indices that make up triangles

material:
float[3] - diffuse color (0..1)
int - shininess (>= 1)
float - shininess strength (>= 0)
texture list - list of textures

texture:
string - filename
string - type
int - uv index

vertex:
float[3] - position
float[3] - normal
float[2] list - uvs *** Note: this list is not prepended with a length int.

*/

