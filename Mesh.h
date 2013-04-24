#pragma once

#include "Vector.h"
#include <vector>
#include <ostream>
#include <iostream>
#include <cstdint>

// Each frame is for an animation frame. Each frame must have num_vertices vertices.
// Every vertex in each triangle indexes a vertex in the current frame.
// Every texcoord in each triangle indexes a texcoord.

class Mesh {
  public:
    // Every frame has the same number of vertices and normals.
    class Frame
    {
      public:
        std::vector<Vector3f> vs; // vertices
        std::vector<Vector3f> ns; // normals
        std::vector<Vector2f> ts; // texture coordinates
    };

    class Triangle
    {
      public:
        int32_t vs[3]; // vertex index
        int32_t ns[3]; // normal index
        int32_t ts[3]; // texture coordinate index
    };

    std::vector<Frame> fs; // frames
    std::vector<Triangle> tris; // triangles
};

bool serialize (std::ostream & out, Mesh const& mesh);
bool deserialize (std::istream & in, Mesh & mesh);

