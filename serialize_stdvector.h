#pragma once

#include "serialize.h"
#include <vector>

/* The function is passed in as a template parameter so the function definition
   can remain constant. */

template <class T, bool serialize_item(std::ostream &, T const&)>
bool serialize (std::ostream & out, std::vector<T> const& v)
{
  bool good = true;
  good = serialize(out, (uint32_t)v.size());
  for (auto i = v.begin(); good && i != v.end(); ++i)
  {
    good = serialize_item(out, *i);
  }
  return good;
}

template <class T, bool deserialize_item(std::istream &, T &)>
bool deserialize (std::istream & in, std::vector<T> & v)
{
  bool good = true;
  uint32_t size;
  good = deserialize(in, size);
  if (good)
  {
    v.clear();
    v.resize(size);
  }
  for (typename std::vector<T>::iterator i = v.begin(); good && i != v.end(); ++i)
  {
    good = deserialize_item(in, *i);
  }
  return good;
}

