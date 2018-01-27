//*****************************************************************************//
//  Matt Garmon ~ 2016                                                         //
//  SC_hash.hpp - Declaration of a Separate Chaining Hash Table                //
//*****************************************************************************//

#ifndef SC_HASH
#define SC_HASH
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>

//hash table class with separate-chaining collision handling
class SC_hash {
private:
    std::vector<std::list<int>> table; //the hash table
    int hasher(int key) {return key%bucket_count();} //primitive hash function
    int keys = 0; //key count
public:
    SC_hash(int); //constructor with size parameter
    int key_count() {return keys;} //returns key count
    int bucket_count() {return table.size();} //returns bucket count
    double load() {return (double)key_count()/bucket_count();} //returns load factor
    void rehash(int); //rehash function
    void insert(int); //insert function
    std::list<int>::iterator find(int); //find function
    void remove(int); //remove function
    void display(); //display function
};

#endif // SC_HASH
