#pragma once

#include "vector.hpp"

// Column-major order

template <class T>
class Table {
public:
          Table ();
          Table (Vector<2, int> const& size);
  virtual ~Table ();

  Vector<2, int> const& get_size () const;
  void                  resize (Vector<2, int> const& size); // erases everything

  T &      operator () (int i);
  T const& operator () (int i) const;
  T &      operator () (Vector<2, int> const& pos);
  T const& operator () (Vector<2, int> const& pos) const;
  T &      operator () (int row, int col);
  T const& operator () (int row, int col) const;

  T &      cell (int i);
  T const& cell (int i) const;
  T &      cell (Vector<2, int> const& pos);
  T const& cell (Vector<2, int> const& pos) const;
  T &      cell (int row, int col);
  T const& cell (int row, int col) const;

private:
  T * c;
  Vector<2, int> size;
};

// Template Implementation

template <class T>
Table<T>::Table() : size(0, 0) {
  c = 0;
}

template <class T>
Table<T>::Table(Vector<2, int> const& size_) : size(size_) {
  if(size(0) > 0 && size(1) > 0)
    c = new T[size(0) * size(1)]();
}

template <class T>
Table<T>::~Table() {
  if(c != 0)
    delete [] c;
}

template <class T>
Vector<2, int> const& Table<T>::get_size() const {
  return size;
}

template <class T>
void Table<T>::resize(Vector<2, int> const& size_) {
  if(c != 0) {
    delete [] c;
    c = 0;
  }
  size = size_;
  if(size(0) > 0 && size(1) > 0)
    c = new T[size(0) * size(1)]();
}

template <class T>
T & Table<T>::operator () (int i) {
  return cell(i);
}

template <class T>
T const& Table<T>::operator () (int i) const {
  return cell(i);
}

template <class T>
T & Table<T>::operator () (Vector<2, int> const& pos) {
  return cell(pos);
}

template <class T>
T const& Table<T>::operator () (Vector<2, int> const& pos) const {
  return cell(pos);
}

template <class T>
T & Table<T>::operator () (int row, int col) {
  return cell(row, col);
}

template <class T>
T const& Table<T>::operator () (int row, int col) const {
  return cell(row, col);
}

template <class T>
T & Table<T>::cell (int i) {
  assert(0 <= i && i < size(0) * size(1));
  return c[i];
}

template <class T>
T const& Table<T>::cell (int i) const {
  assert(0 <= i && i < size(0) * size(1));
  return c[i];
}

template <class T>
T & Table<T>::cell (Vector<2, int> const& pos) {
  assert(0 <= pos(1) && pos(1) < size(1) && 0 <= pos(0) && pos(0) < size(0));
  return c[pos(1) * size(1) + pos(0)];
}

template <class T>
T const& Table<T>::cell (Vector<2, int> const& pos) const {
  assert(0 <= pos(1) && pos(1) < size(1) && 0 <= pos(0) && pos(0) < size(0));
  return c[pos(1) * size(1) + pos(0)];
}

template <class T>
T & Table<T>::cell (int row, int col) {
  assert(0 <= row && row < size(1) && 0 <= col && col < size(0));
  return c[col * size(1) + row];
}

template <class T>
T const& Table<T>::cell (int row, int col) const {
  assert(0 <= row && row < size(1) && 0 <= col && col < size(0));
  return c[col * size(1) + row];
}

