#include "Mesh.h"
#include "serialize_stdvector.h"

bool serialize (std::ostream & out, Mesh::Frame const& frame)
{
  bool good = true;
  good = serialize<Vector3f, serialize> (out, frame.vs);
  if (good)
  {
    good = serialize<Vector3f, serialize> (out, frame.ns);
  }
  if (good)
  {
    good = serialize<Vector2f, serialize> (out, frame.ts);
  }
  return good;
}

bool deserialize (std::istream & in, Mesh::Frame & frame)
{
  bool good = true;
  good = deserialize<Vector3f, deserialize> (in, frame.vs);
  if (good)
  {
    good = deserialize<Vector3f, deserialize> (in, frame.ns);
  }
  if (good)
  {
    good = deserialize<Vector2f, deserialize> (in, frame.ts);
  }
  return good;
}

bool serialize (std::ostream & out, Mesh const& mesh)
{
  bool good = true;
  good = serialize<Mesh::Frame, serialize> (out, mesh.fs);
  if (good)
  {
    good = serialize<Mesh::Triangle, serialize> (out, mesh.tris);
  }
  return good;
}

bool deserialize (std::istream & in, Mesh & mesh)
{
  bool good = true;
  good = deserialize<Mesh::Frame, deserialize> (in, mesh.fs);
  if (good)
  {
    good = deserialize<Mesh::Triangle, deserialize> (in, mesh.tris);
  }
  return good;
}

