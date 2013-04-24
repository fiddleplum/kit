#pragma once

#include "vector.hpp"
#include "serialize.hpp"

template <int dim, class T>
void serialize (std::ostream & out, Vector<dim, T> const& v) {
  for(int i = 0; i < dim; ++i)
    serialize(out, v[i]);
}

template <int dim, class T>
void deserialize (std::istream & in, Vector<dim, T> & v) {
  for(int i = 0; i < dim; ++i)
    deserialize(in, v[i]);
}