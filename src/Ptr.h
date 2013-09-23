#pragma once

// Dependencies
#include <cassert>

// Forward declarations
template <class T>
class UsePtr;
class _OwnPtrCounter;

//! This is a function that can be passed to OwnPtr to delete arrays properly.
template <class T>
void destroyArray(T * p)
{
  delete [] p;
}

//! OwnPtr is a smart pointer that controls ownership. It is copyable and maintains an OwnPtr reference count, that when the last OwnPtr reference of a particular object is deconstructed, the pointer is destroyed. When the last OwnPtr of a pointer is being destroyed, there must be no UsePtrs pointing to it. If there are, it will fail an assertion. This ensures that the OwnPtr and object always exist at least as long as the UsePtrs. OwnPtr can take a destroy function that must dispose of the pointer appropriately.
/**
	In reference to cycles: The solution is to unlink all objects first, and then delete them. For doubly-linked trees, unlink the upward links first. For doubly linked lists, unlink all connections, etc.
**/

template <class T>
class OwnPtr
{
public:
  //! Initialize the pointer to null. It can take a function that destroys the object(default is the standard delete operator).
  OwnPtr(void(*destroy)(T *) = nullptr);

  //! Initialize the pointer to newP, which can have a type that is subclass of T. Only pass in something that looks like 'new T()' to ensure that the raw pointer isn't used elsewhere. It can also take a function that destroys the object (default is the standard delete operator).
  template <class Y> explicit OwnPtr(Y * newP, void(* destroy)(T *) = nullptr);

  //! Default copy constructor. Needed otherwise C++ will create its own.
  OwnPtr(OwnPtr<T> const & ptr);

  //! Templated copy constructor. It can take a pointer that has a type that is a subclass of T.
  template <class Y> OwnPtr(OwnPtr<Y> const & ptr);

  //! Destructor. If this is the last OwnPtr reference to the object, either delete is called or the destroy function is called if it is specified. There must be no UsePtrs pointing to the object. Note that it is not virtual, so don't subclass OwnPtr.
  ~OwnPtr();

  //! Default assignment operator. Needed otherwise C++ will create its own.
  OwnPtr<T> & operator = (OwnPtr<T> const & ptr);

  //! Templated assignment operator. It can take a pointer that has a type that is a subclass of T.
  template <class Y> OwnPtr<T> & operator = (OwnPtr<Y> const & ptr);

  //! Returns true if this points to zero.
  bool isNull() const;

  //! Returns true if this points to something non-zero.
  bool isValid() const;

  //! Returns true if there is at least one UsePtr that points to the object this points to.
  bool isUsed() const;

  //! Change the object that this points to newP, which can have a type that is subclass of T. Only pass in something that looks like 'new T()' to ensure that the raw pointer isn't used elsewhere. If there was a previous object pointed to, the same algorithm as the destructor is called.
  template <class Y> void set(Y * newP, void(*newDestroy)(T *) = nullptr);

  //! Resets the object to point to nothing. If there was a previous object pointed to, the same algorithm as the destructor is called.
  void setNull();

  //! Provides access to the object's members.
  T * operator ->() const;

  //! Provides access to the element located at index. Warning: this provides no index out-of-bounds checking.
  T & operator [](int index) const;

  //! Provides reference access to the object.
  T & operator *() const;

  //! Only to be used for functions that require a pointer. Be careful how you use this.
  T * raw() const;

  //! Get the unsigned integer value of the pointer's object.
  operator unsigned int() const;

  //! Returns a use pointer statically casted to Y.
  template <class Y> UsePtr<Y> staticCast() const;

  //! Returns a use pointer dynamically casted to Y.
  template <class Y> UsePtr<Y> dynamicCast() const;

  //! Returns true if the address of this object is less than the address of ptr's object.
  bool operator <(OwnPtr<T> const & ptr) const;

  //! Returns true if the address of this object is less than the address of ptr's object.
  bool operator <(UsePtr<T> const & ptr) const;

  //! Returns true if the address of this object is equal to the address of ptr's object.
  bool operator == (OwnPtr<T> const & ptr) const;

  //! Returns true if the address of this object is equal to the address of ptr's object.
  bool operator == (UsePtr<T> const & ptr) const;

private:
  T * p;
  _OwnPtrCounter * c;
  void(* destroy)(T *);
  template <class Y> friend class OwnPtr;
  template <class Y> friend class UsePtr;
};

//! UsePtr is a smart pointer that has no ownership and so never deletes the pointer that it references. The object it points to is guaranteed to exist while the UsePtr points to it.

template <class T>
class UsePtr
{
public:
  //! Default constructor. Initializes the pointer to null.
  UsePtr();

  //! Default copy constructor. Needed otherwise C++ will create its own.
  UsePtr(UsePtr<T> const & ptr);

  //! Templated copy constructor.
  template <class Y> UsePtr(UsePtr<Y> const & ptr);

  //! Initializes this to point to the same object that ptr points to. While this exists and points to the object, it has a guarantee that the object exists.
  template <class Y> UsePtr(OwnPtr<Y> const & ptr);

  //! Destructor.
  ~UsePtr();

  //! Default assignment operator. Needed, otherwise C++ will create its own.
  UsePtr<T> & operator = (UsePtr<T> const & ptr);

  //! Templated assignment operator.
  template <class Y> UsePtr<T> & operator = (UsePtr<Y> const & ptr);

  //! Assigns this to point to the same object that ptr points to. If this pointed to a previous object, then that reference is removed. While this exists and points to the new object, it has a guarantee that the object exists.
  template <class Y> UsePtr<T> & operator = (OwnPtr<Y> const & ptr);

  //! Returns true if this points to zero.
  bool isNull() const;

  //! Returns true if this points to something non-zero.
  bool isValid() const;

  //! Makes this point to nothing.
  void setNull();

  //! Provides access to the object's members.
  T * operator ->() const;

  //! Provides access to the element located at index. Warning: this provides no index out-of-bounds checking.
  T & operator [](int index) const;

  //! Provides reference access to the object.
  T & operator *() const;

  //! Get the unsigned integer value of the pointer's object.
  operator unsigned int() const;

  //! Only to be used for functions that require a pointer. Be careful how you use this.
  T * raw() const;

  //! Returns a use pointer statically casted to Y.
  template <class Y> UsePtr<Y> staticCast() const;

  //! Returns a use pointer dynamically casted to Y.
  template <class Y> UsePtr<Y> dynamicCast() const;

  //! Returns true if the address of this object is less than the address of ptr's object.
  bool operator <(UsePtr<T> const & ptr) const;

  //! Returns true if the address of this object is less than the address of ptr's object.
  bool operator <(OwnPtr<T> const & ptr) const;

  //! Returns true if the address of this object is equal to the address of ptr's object.
  template <class Y> bool operator == (UsePtr<Y> const & ptr) const;

  //! Returns true if the address of this object is equal to the address of ptr's object.
  template <class Y> bool operator == (OwnPtr<Y> const & ptr) const;

private:
  T * p;
  int * cc; // contract reference counter
  template <class Y> friend class UsePtr;
  template <class Y> friend class OwnPtr;
};

// Template Implementation.

class _OwnPtrCounter
{
public:
  int oc; // owner reference counter
  int cc; // use reference counter
};

template <class T>
OwnPtr<T>::OwnPtr(void(*destroy)(T *)) : p(nullptr), c(nullptr), destroy(destroy)
{
}

template <class T> template <class Y>
OwnPtr<T>::OwnPtr(Y * newP, void(*destroy)(T *)) : p(newP), destroy(destroy)
{
  if(p != nullptr)
  {
    c = new _OwnPtrCounter;
    c->oc = 1;
    c->cc = 0;
  }
  else
  {
    c = nullptr;
  }
}

template <class T>
OwnPtr<T>::OwnPtr(OwnPtr<T> const & ptr) : p(ptr.p), c(ptr.c), destroy(ptr.destroy)
{
  if(p != 0)
  {
    c->oc++;
  }
}

template <class T> template <class Y>
OwnPtr<T>::OwnPtr(OwnPtr<Y> const & ptr) : p(ptr.p), c((_OwnPtrCounter *)ptr.c), destroy((void(*)(T *))ptr.destroy)
{
  if(p != nullptr)
  {
    c->oc++;
  }
}

template <class T>
OwnPtr<T>::~OwnPtr()
{
  setNull();
}

template <class T>
OwnPtr<T> & OwnPtr<T>::operator = (OwnPtr<T> const & ptr)
{
  if(p == ptr.p)
  {
    return *this;
  }
  setNull();
  // Set new pointer
  p = ptr.p;
  c = ptr.c;
  if(p != nullptr)
  {
    c->oc++;
  }
  return *this;
}

template <class T> template <class Y>
OwnPtr<T> & OwnPtr<T>::operator = (OwnPtr<Y> const & ptr)
{
  if(p == ptr.p)
  {
    return *this;
  }
  setNull();
  // Set new pointer
  p = ptr.p;
  c = ptr.c;
  if(p != nullptr)
  {
    c->oc++;
  }
  return *this;
}

template <class T>
bool OwnPtr<T>::isNull() const
{
  return p == nullptr;
}

template <class T>
bool OwnPtr<T>::isValid() const
{
  return p != nullptr;
}

template <class T>
bool OwnPtr<T>::isUsed() const
{
  assert(p != nullptr);
  return c->cc != 0;
}

template <class T> template <class Y>
void OwnPtr<T>::set(Y * newP, void(*newDestroy)(T *))
{
  setNull();
  destroy = newDestroy;
  p = newP;
  if(p != nullptr)
  {
    c = new _OwnPtrCounter;
    c->oc = 1;
    c->cc = 0;
  }
  else
  {
    c = nullptr;
  }
}

template <class T>
void OwnPtr<T>::setNull()
{
  if(p != nullptr)
  {
    c->oc--;
    if(c->oc == 0)
    {
      assert(c->cc == 0);
      if(destroy != nullptr)
      {
        destroy(p);
      }
      else
      {
        delete p;
      }
      delete c;
    }
    p = nullptr;
    c = nullptr;
  }
}

template <class T>
T * OwnPtr<T>::operator ->() const
{
  assert(p != nullptr);
  return p;
}

template <class T>
T & OwnPtr<T>::operator [](int index) const
{
  assert(p != nullptr);
  return p[index];
}

template <class T>
T & OwnPtr<T>::operator *() const
{
  assert(p != nullptr);
  return *p;
}

template <class T>
T * OwnPtr<T>::raw() const
{
  assert(p != nullptr);
  return p;
}

template <class T>
OwnPtr<T>::operator unsigned int() const
{
  return(unsigned int)p;
}

template <class T> template <class Y> UsePtr<Y> OwnPtr<T>::staticCast() const
{
  UsePtr<Y> cp;
  cp.p = static_cast<Y *>(p);
  cp.cc = &c->cc;
  if(p != nullptr)
  {
    c->cc++;
  }
  return cp;
}

template <class T> template <class Y> UsePtr<Y> OwnPtr<T>::dynamicCast() const
{
  UsePtr<Y> cp;
  cp.p = dynamic_cast<Y *>(p);
  cp.cc = &c->cc;
  if(p != nullptr)
  {
    c->cc++;
  }
  return cp;
}

template <class T>
bool OwnPtr<T>::operator <(OwnPtr<T> const & ptr) const
{
  return p < ptr.p;
}

template <class T>
bool OwnPtr<T>::operator <(UsePtr<T> const & ptr) const
{
  return p < ptr.p;
}

template <class T>
bool OwnPtr<T>::operator == (OwnPtr<T> const & ptr) const
{
  return p == ptr.p;
}

template <class T>
bool OwnPtr<T>::operator == (UsePtr<T> const & ptr) const
{
  return p == ptr.p;
}

template <class T>
UsePtr<T>::UsePtr() : p(nullptr), cc(nullptr)
{
}

template <class T>
UsePtr<T>::UsePtr(UsePtr<T> const & ptr) : p(ptr.p), cc(ptr.cc)
{
  if(p != nullptr)
  {
   (*cc)++;
  }
}

template <class T> template <class Y>
UsePtr<T>::UsePtr(UsePtr<Y> const & ptr) : p(ptr.p), cc(ptr.cc)
{
  if(p != nullptr)
  {
   (*cc)++;
  }
}

template <class T> template <class Y>
UsePtr<T>::UsePtr(OwnPtr<Y> const & ptr) : p(ptr.p), cc(&ptr.c->cc)
{
  if(p != nullptr)
  {
   (*cc)++;
  }
}

template <class T>
UsePtr<T>::~UsePtr()
{
  if(p != nullptr)
  {
   (*cc)--;
  }
}

template <class T>
UsePtr<T> & UsePtr<T>::operator = (UsePtr<T> const & ptr)
{
  if(p == ptr.p)
  {
    return *this;
  }
  if(p != nullptr)
  {
   (*cc)--;
  }
  p = ptr.p;
  cc = ptr.cc;
  if(p != nullptr)
  {
   (*cc)++;
  }
  return *this;
}

template <class T> template <class Y>
UsePtr<T> & UsePtr<T>::operator = (UsePtr<Y> const & ptr)
{
  if(p == ptr.p)
  {
    return *this;
  }
  if(p != nullptr)
  {
   (*cc)--;
  }
  p = ptr.p;
  cc = ptr.cc;
  if(p != nullptr)
  {
   (*cc)++;
  }
  return *this;
}

template <class T> template <class Y>
UsePtr<T> & UsePtr<T>::operator = (OwnPtr<Y> const & ptr)
{
  if(p == ptr.p)
  {
    return *this;
  }
  if(p != nullptr)
  {
   (*cc)--;
  }
  p = ptr.p;
  cc = &ptr.c->cc;
  if(p != nullptr)
  {
   (*cc)++;
  }
  return *this;
}

template <class T>
bool UsePtr<T>::isNull() const
{
  return p == nullptr;
}

template <class T>
bool UsePtr<T>::isValid() const
{
  return p != nullptr;
}

template <class T>
void UsePtr<T>::setNull()
{
  if(p != nullptr)
  {
   (*cc)--;
  }
  p = nullptr;
  cc = nullptr;
}

template <class T>
T * UsePtr<T>::operator ->() const
{
  assert(p != nullptr);
  return p;
}

template <class T>
T & UsePtr<T>::operator [](int index) const
{
  assert(p != nullptr);
  return p[index];
}

template <class T>
T & UsePtr<T>::operator *() const
{
  assert(p != nullptr);
  return *p;
}

template <class T>
T * UsePtr<T>::raw() const
{
  assert(p != nullptr);
  return p;
}

template <class T>
UsePtr<T>::operator unsigned int() const
{
  return(unsigned int)p;
}

template <class T> template <class Y> UsePtr<Y> UsePtr<T>::staticCast() const
{
  UsePtr<Y> cp;
  cp.p = static_cast<Y *>(p);
  cp.cc = cc;
  if(p != nullptr)
  {
   (*cc)++;
  }
  return cp;
}

template <class T> template <class Y> UsePtr<Y> UsePtr<T>::dynamicCast() const
{
  UsePtr<Y> cp;
  cp.p = dynamic_cast<Y *>(p);
  cp.cc = cc;
  if(p != nullptr)
  {
   (*cc)++;
  }
  return cp;
}

template <class T>
bool UsePtr<T>::operator <(UsePtr<T> const & ptr) const
{
  return p < ptr.p;
}

template <class T>
bool UsePtr<T>::operator <(OwnPtr<T> const & ptr) const
{
  return p < ptr.p;
}

template <class T> template <class Y>
bool UsePtr<T>::operator == (UsePtr<Y> const & ptr) const
{
  return p == ptr.p;
}

template <class T> template <class Y>
bool UsePtr<T>::operator == (OwnPtr<Y> const & ptr) const
{
  return p == ptr.p;
}

