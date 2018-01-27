
// (c) Andrew Sutton 2016
// All rights reserved
//
// This module provides utility routines for raw storage allocation
// and the construction and destruction of objects into uninitialized
// storage.

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <new>     // Includes ::operator new and ::operator delete
#include <utility> // Includes std::move.


// Allocate storage for n objects of type T. Note that this does not
// construct objects in that storage.
//
// Example:
//
//    int* p = allocate<int>(20);
//
// This allocates space for 20 integers. Use construct() to create objects
// in uninitialized memory.
template<typename T>
inline T*
allocate(int n)
{
  return reinterpret_cast<T*>(::operator new(n * sizeof(T)));
}


// Deallocate memory allocated using the allocate() function. Be sure to
// destroy the objects prior to deallocation.
template<typename T>
inline void
deallocate(T* p)
{
  ::operator delete(p);
}


// Construct an object in at the storage pointed to by address p. You can
// provide any arguments you want to the constructor, provided that T has
// a constructor that accepts those arguments.
//
// Example:
//
//    int* p = allocate<int>(1); // Allocate storage for one int
//    construct(p, 5);           // Construct an object at p with value 5
//
template<typename T, typename... Args>
inline T*
construct(T* p, Args&&... args)
{
  return new (p) T(std::forward<Args>(args)...);
}


// Destroy the object pointed to by p, but do not reclaim its memory. Note
// that the object must have been created in raw memory using the construct()
// function above.
//
// Example:
//
//    int* p = ... // An object created using construct().
//    destroy(p);  // Destroy the object.
//
template<typename T>
inline void
destroy(T* p)
{
  p->~T();
}


// Construt a copy of each element in [first, last) in the storage
// pointed to by out. After execution, [out, out + (last - first))
// will be a range of objects equal to those in [first, last). This
// returns a pointer past the last copy-constructed object.
template<typename T>
inline T*
uninitialized_copy(T const* first, T const* last, T* out)
{
  while (first != last) {
    construct(out, *first);
    ++out;
    ++first;
  }
  return out;
}


// Same as above, but mvoes the objects in [first, last) into the
// storage pointed at by out.
template<typename T>
inline T*
uninitialized_move(T const* first, T const* last, T* out)
{
  while (first != last) {
    construct(out, std::move(*first));
    ++out;
    ++first;
  }
  return out;
}


// Destroy the elements in the range [first, last). This does
// not release the underlying memory.
template<typename T>
inline T*
initialized_destroy(T* first, T* last)
{
  while (first != last) {
    destroy(first);
    ++first;
  }
  return first;
}


#endif
