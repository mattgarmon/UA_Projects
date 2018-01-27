//*****************************************************************************//
//  Matt Garmon ~ 2016                                                         //
//  SC_hash.cpp - Implementation of a Separate Chaining Hash Table             //
//*****************************************************************************//

#include "SC_hash.hpp"

//constructor
SC_hash::SC_hash(int n) {
    if (n <= 0) n = 16; //default size is 16 buckets
    table.resize(n);
    for(int i = 0; i < n; ++i) //initialize all buckets to empty
        table[i] = std::list<int>(1, -1);
}

//function to insert a key
void SC_hash::insert(int key) {
    ++keys; //update key count
    if(load() > .75) rehash(2*bucket_count()); //check load factor, possibly rehash
    int code = hasher(key); //get the hash code
    if(table[code].front() == -1) //insert the key into the correct bucket
        table[code].pop_front();
    table[code].push_back(key);
}

//function to find a key
std::list<int>::iterator SC_hash::find(int key) {
    int code = hasher(key); //get the hash code
    //find the key, return iterator to position, last if not found
    return std::find(table[code].begin(), table[code].end(), key);
}

//function to remove a key
void SC_hash::remove(int key) {
    int code = hasher(key); //get the hash code
    if(table[code].front() != -1) table[code].remove(key); //remove the key
    --keys; //update key count
}

//function to rehash the table
void SC_hash::rehash(int n) {
    if(n <= bucket_count()) return; //do nothing
    std::list<int> temp; //create a temporary list to hold keys
    std::list<int>::iterator iter; //get a list iterator
    for(size_t i = 0; i < table.size(); ++i) {
        if(table[i].front() == -1) {} //skip empty buckets
        else {
            for(iter = table[i].begin(); iter != table[i].end(); ++iter)
                temp.push_back(*iter); //push all the keys into the list
        }
    }
    table.resize(n); //update the table size
    for(int i = 0; i < n; ++i) //initialize all buckets to empty
        table[i] = std::list<int>(1, -1);
    keys = 0;
    for(iter = temp.begin(); iter != temp.end(); ++iter)
        insert(*iter); //re-insert all of the keys
}

//function to display the hash table
void SC_hash::display() {
    for(size_t i = 0; i < table.size(); ++i) {
        std::cout << i << (i < 10 ? " ] " :  "] ");
        std::list<int>::const_iterator iter;
        for(iter = table[i].begin(); iter != table[i].end(); ++iter)
            std::cout << *iter << ' ';
        std::cout << std::endl;
    }
    std::cout << '\n';
}
