#include <iostream>
#include <vector>
#include <list>

//The print function
template<class Iter>
void print(Iter first, Iter limit)
{
  if (first == limit) return;
  std::cout << *first << ' ';
  print(++first, limit);
}

//The find function
template<class Iter>
Iter find(Iter first, Iter limit, int value) {
  if (first == limit || *first == value) return first;
  return find(++first, limit, value);
}

//The in function
template<class Iter>
bool in(Iter first, Iter limit, int value) {
  if (*find(first, limit, value) == value) return true;
  else return false;
}

//The count function
template<class Iter>
int count(Iter first, Iter limit, int value) {
  if (first == limit) return 0;
  if (*first == value) return 1 + count(++first, limit, value);
  else return count(++first, limit, value);
}

//The equal function
template<class Iter1, class Iter2>
bool equal(Iter1 first1, Iter1 limit1, Iter2 first2, Iter2 limit2) {
  if (std::distance(first1, limit1) != std::distance(first2, limit2)) {
    std::cout << "The range sizes are not equal" << std::endl;
    return false;
  }
  if (first1 == limit1) return true;
  if (*first1 == *first2) return equal(++first1, limit1, ++first2, limit2);
  else return false;
}

//The first minimum function
int minimum(int a, int b) {
  return (a < b) ? a : b;
}

//The second minimum function
template<class Iter>
Iter minimum(Iter first, Iter limit) {
  if (first == limit) return first;
  *first = minimum(*first, *limit);
  return minimum(first, --limit);
}

//The first maximum function
int maximum(int a, int b) {
  return (a > b) ? a : b;
}

//The second maximum function
template<class Iter>
Iter maximum(Iter first, Iter limit) {
  if (first == limit) return first;
  *first = maximum(*first, *limit);
  return maximum(first, --limit);
}

//The compare function
template<class Iter1, class Iter2>
int compare(Iter1 first1, Iter1 limit1, Iter2 first2, Iter2 limit2) {
    if (equal(first1, limit1, first2, limit2) || first1 == limit1 || first2 == limit2)
      return 0;
    if (*first1 < *first2) return -1;
    if (*first1 > *first2) return 1;
    return compare(++first1, limit1, ++first2, limit2);
}

//The copy function
template<class Iter1, class Iter2>
void copy(Iter1 first1, Iter1 limit1, Iter2 first2, Iter2 limit2) {
  if (std::distance(first1, limit1) != std::distance(first2, limit2)) {
    std::cout << "The range sizes are not equal" << std::endl;
    return;
  }
  if (first1 == limit1) return;
  *first1 = *first2;
  return copy(++first1, limit1, ++first2, limit2);
}

//The fill function
template<class Iter>
void fill(Iter first, Iter limit, int value) {
  if (first == limit) return;
  *first = value;
  return fill(++first, limit, value);
}

//The reverse function
template<class Iter>
void reverse(Iter first, Iter limit) {
  --limit;
  if (first == limit || first == limit - 1) return;
  std::swap(*first, *limit);
  return reverse(++first, limit);
}

int main() {
  //Creating the vector and list, and their iterators
  std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::list<int> b = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
  std::vector<int>::iterator ab = a.begin();
  std::vector<int>::iterator ae = a.end();
  std::list<int>::iterator bb = b.begin();
  std::list<int>::iterator be = b.end();

  //Testing the print function for a and b
  std::cout << "=======================================================" << std::endl;
  std::cout << "Vector a: ";
  print(ab, ae);
  std::cout << std::endl;
  std::cout << "List b: ";
  print(bb, be);
  std::cout << std::endl;

  //Testing the find function for both cases
  std::cout << "=======================================================" << std::endl;
  std::cout << "Searching for 4 from (a) to (a + 7): " << *find(ab, ab + 7, 4) << std::endl;
  std::cout << "Searching for 40 from (a) to (a + 7): " << *find(ab, ab + 7, 40) << std::endl;

  //Testing the in function for both cases
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "The value 4 does" << (in(ab, ab + 7, 4) ? "" : " not")
            << " exist somewhere in (a) to (a + 7)" << std::endl;
  std::cout << "The value 40 does" << (in(ab, ab + 7, 40) ? "" : " not")
            << " exist somewhere in (a) to (a + 7)" << std::endl;

  //Testing the count function
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "The value 4 was found " << count(ab, ab + 7, 4)
            << " time(s) from (a) to (a + 7)" << std::endl;

  //Testing the equal function
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "The ranges a and b are " << (equal(ab, ae, bb, be) ? "" : "not ")
            << "equal" << std::endl;

  //Testing the first minimum function
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "The minimum of 11 and 7 is: " << minimum (11, 7) << std::endl;

  //Testing the second minimum function for both cases
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "Searching for minimum from (a) to (a + 7): "
            << *minimum(ab, ab + 7) << std::endl;
  std::cout << "Searching for minimum from (a + 1) to (a + 1): "
            << *minimum(ab + 1, ab + 1) << std::endl;

  //Testing the first maximum function
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "The maximum of 11 and 7 is: " << maximum(11, 7) << std::endl;

  //Testing the second maximum function for both cases
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "Searching for maximum from (a) to (a + 7): "
            << *maximum(ab, ab + 7) << std::endl;
  std::cout << "Searching for maximum from (a + 1) to (a + 1): "
            << *maximum(ab + 1, ab + 1) << std::endl;


  //Testing the compare function for each case
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "Comparing (a) to (a + 5) and (a + 1) to (a + 6): "
            << compare(ab, ab + 5, ab + 1, ab + 6) << std::endl;
  std::cout << "Comparing (a) to (a + 5) and (a) to (a + 5): "
            << compare(ab, ab + 5, ab, ab + 5) << std::endl;
  std::cout << "Comparing (a + 1) to (a + 6) and (a) to (a + 5): "
            << compare(ab + 1, ab + 6, ab, ab + 5) << std::endl;

  //Testing the copy function
  std::cout << "-------------------------------------------------------" << std::endl;
  copy(ab, ab + 7, bb, std::next(bb, 7));
  std::cout << "After copying (b) to (b + 7) into (a) to (a + 7)...\n"
            << "Vector a is now: ";
  print(ab, a.end());
  std::cout << std::endl;

  //Testing the fill function
  std::cout << "-------------------------------------------------------" << std::endl;
  fill(std::next(bb, 1), std::next(bb, 8), 27);
  std::cout << "After filling (b + 1) to (b + 8) with 27...\n"
            << "List b is now: ";
  print(bb, be);
  std::cout << std::endl;

  //Testing the reverse function function
  std::cout << "-------------------------------------------------------" << std::endl;
  reverse(ab, ae);
  std::cout << "After reversing a...\n"
            << "Vector a is now: ";
  print(ab, ae);
  std::cout << std::endl;
  std::cout << "-------------------------------------------------------" << std::endl;

  return 0;
}

