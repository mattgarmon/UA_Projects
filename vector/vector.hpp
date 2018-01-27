
#ifndef VECTOR_HPP
#define VECTOR_HPP
#include "memory.hpp"
#include <initializer_list>
#include <cassert>

template <class T>
struct Vector {
  T* base = nullptr; // Points to the first element
  T* last = nullptr; // Points after the last element
  T* limit = nullptr; // Points after the storage

  Vector() {} // Default constructor
  Vector(std::initializer_list<T>); // Constructor for a list
  Vector(const Vector&); // The copy constructor
  ~Vector(); // Destructor

  Vector<T>& operator=(Vector<T>&); // Copy assignment operator
  bool empty() const; // Checks if the Vector is empty
  std::size_t size() const {return last - base;} // Returns the size
  std::size_t capacity() const {return limit - base;} // Returns the capacity
  T* data() const {return base;} // Returns a pointer to first element
  T& operator[](std::size_t); // Element mutate operator
  T operator[](std::size_t) const; // Element access operator
  void reserve(std::size_t); // Reserves capacity for elements
  void resize(std::size_t); // Resizes the Vector
  void clear(); // Empties the Vector
  void display();

  using iterator = T*; // Iterators
  using const_iterator = T*;
  iterator begin() { return base; }
  iterator end() { return last; }
  const_iterator begin() const { return base; }
  const_iterator end() const { return last; }

  void push_back(T); // Adds to the back of the Vector
  void pop_back(); // Takes off the back of the Vector

  void push_front(T); // Adds to the front of the Vector
  void pop_front(); // Takes off the front of the Vector

  T front() const { return *base; } // Returns the front of the Vector as a constant
  T& front() { return *base; } // Returns the front of the Vector
};
template <class T>
void Vector<T>::display() {
  std::cout << "\n-------------------------------------"
	    << "\nSize: " << size()
	    << "\nCapacity: " << capacity()
	    << "\nElements: ";
  for(T* temp = base; temp < last; std::cout << "[\"" << *(temp++) << "\"]");
  std::cout << "\n-------------------------------------\n";
}

// Constructor for a list
template <class T>
Vector<T>::Vector(std::initializer_list<T> list)
  : base(), last(), limit() {
  reserve(list.size());
  for (T const& t : list)
    push_back(t);
}

// Copy constructor
template <class T>
Vector<T>::Vector(const Vector& v) {
  reserve(v.size());
  for (T* temp = v.base; temp < v.last;
    construct(last++, T(*(temp++))));
}

// Destructor
template <class T>
Vector<T>::~Vector() {
  clear();
  deallocate(base);
}

// Copy assignment operator
template <class T>
Vector<T>& Vector<T>::operator=(Vector<T>& v) {
  Vector temp(v);
  std::swap(base, temp.base);
  std::swap(last, temp.last);
  std::swap(limit, temp.limit);
  return *this;
}

// Checks if the Vector is empty
template <class T>
bool Vector<T>::empty() const {
  T* temp = base;
  while (temp < last)
   if (temp++) return false;
  return true;
}

// Element mutate operator
template <class T>
T& Vector<T>::operator[](std::size_t i) {
  assert(i < size() && i >= 0);
  return *(base + i);
}

// Element access operator
template<class T>
T Vector<T>::operator[](std::size_t i) const {
  assert(i < size() && i >= 0);
  return *(base + i);
}

// Reserve storage
template <class T>
void Vector<T>::reserve(std::size_t c) {
  if (capacity() >= c) return;
  if (!base) {
    last = base = allocate<T>(c);
    limit = base + c;
  } else {
    T* new_base = allocate<T>(c);
    T* new_last = new_base;
    limit = new_base + c;
    for (T* temp = base; temp < last;
         construct(new_last++, T(*(temp++))));
    clear();
    deallocate(base);
    base = new_base;
    last = new_last;
  }
}

// Resize the Vector
template <class T>
void Vector<T>::resize(std::size_t n) {
  assert(n >= 0);
  if (n > size()){
    reserve(n);
    static T t;
    for (std::size_t i = size(); (i++) < n; push_back(t));
  } else for (std::size_t i = size(); (i--) > n; destroy(last--));
}

// Empty the Vector
template <class T>
void Vector<T>::clear() {
  while(last > base)
    destroy(--last);
}

// Adds to the back of the Vector
template <class T>
void Vector<T>::push_back(T t) {
  if (last == limit) reserve(capacity() == 0 ? 8 : capacity() * 2);
  construct(last++, T(t));
}

// Takes off of the back of the Vector
template <class T>
void Vector<T>::pop_back() {
  assert(last != base);
  destroy(--last);
}

// Adds to the front of the Vector
template <class T>
void Vector<T>::push_front(T t) {
  if (last == limit) reserve(capacity() == 0 ? 8 : capacity() * 2);
  else reserve(1);
  for (T* temp = last; temp > base; --temp)
    *temp = *(temp - 1);
  ++last;
  construct(base, T(t));
}

// Takes off of the front of the Vector
template <class T>
void Vector<T>::pop_front() {
  assert(last != base);
  for (T* temp = base; temp < last; ++temp)
    *temp = *(temp + 1);
  destroy(--last);
}

// Equality comparisons
template <class T>
bool operator==(const Vector<T>& v1, const Vector<T>& v2) {
  assert(v1.size() == v2.size());
  T* temp1 = v1.base;
  T* temp2 = v2.base;
  while(temp1 < v1.last)
    if (*(temp1++) != *(temp2++)) return false;
  return true;
}
template <class T>
bool operator!=(const Vector<T>& v1, const Vector<T>& v2) {
  return !(v1 == v2);
}

// Ordering operators
template <class T>
bool operator<(const Vector<T>& v1, const Vector<T>& v2) {
  assert(v1.size() == v2.size());
  T* temp1 = v1.base;
  T* temp2 = v2.base;
  while(temp1 < v1.last)
    if (*(temp1++) >= *(temp2++)) return false;
  return true;
}
template <class T>
bool operator>(const Vector<T>& v1, const Vector<T>& v2) {
  assert(v1.size() == v2.size());
  T* temp1 = v1.base;
  T* temp2 = v2.base;
  while(temp1 < v1.last)
    if (*(temp1++) <= *(temp2++)) return false;
  return true;
}
template <class T>
bool operator<=(const Vector<T>& v1, const Vector<T>& v2) {
  assert(v1.size() == v2.size());
  T* temp1 = v1.base;
  T* temp2 = v2.base;
  while(temp1 < v1.last)
    if (*(temp1++) > *(temp2++)) return false;
  return true;
}
template <class T>
bool operator>=(const Vector<T>& v1, const Vector<T>& v2) {
  assert(v1.size() == v2.size());
  T* temp1 = v1.base;
  T* temp2 = v2.base;
  while(temp1 < v1.last)
    if (*(temp1++) < *(temp2++)) return false;
  return true;
}

#endif

