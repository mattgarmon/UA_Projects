//*****************************************************************************//
//  Matt Garmon ~ 2016                                                         //
//  OA_hash.hpp - Declaration of an Open-Addressing Hash Table                 //
//*****************************************************************************//

#ifndef OA_HASH
#define OA_HASH
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

//hash table class with open-addressing collision handling
class OA_hash {
private:
    std::vector<int> table; //the hash table
    int hasher(int key) {return key%bucket_count();} //primitive hash function
    int keys = 0; //key count
public:
    OA_hash(int); //constructor with size parameter
    int key_count() {return keys;} //returns key count
    int bucket_count() {return table.size();} //returns bucket count
    double load() {return (double)key_count()/bucket_count();} //returns load factor
    void rehash(int); //rehash function
    void insert(int); //insert function
    std::vector<int>::iterator find(int); //find function
    void remove(int); //remove function
    void display(); //display function
};

#endif // OA_HASH
