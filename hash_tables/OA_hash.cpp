//*****************************************************************************//
//  Matt Garmon ~ 2016                                                         //
//  OA_hash.cpp - Implementation of an Open-Addressing Hash Table              //
//*****************************************************************************//

#include "OA_hash.hpp"

//constructor
OA_hash::OA_hash(int n) {
    if (n <= 0) n = 16; //default size is 16 buckets
    table.resize(n);
    for(int i = 0; i < n; ++i) //initialize all buckets to empty
        table[i] = -1;
}

//function to insert a key
void OA_hash::insert(int key) {
    ++keys; //update key count
    if(load() > .75) rehash(2*bucket_count()); //check load factor, possibly rehash
    int code = hasher(key); //get the hash code
    //insert the key into the target bucket
    if(table[code] == -1 || table[code] == -2) table[code] = key;
    else { //the bucket was taken, initiate probing
        for(size_t i = code + 1; i < table.size(); ++i) {
            if(table[i] == -1 || table[i] == -2) {
                table[i] = key; //insert the key
                return;
            }
        }
        for(size_t i = 0; i < code; ++i) {
            if(table[i] == -1 || table[i] == -2) {
                table[i] = key; //insert the key
                return;
            }
        }
    }
}

//function to find a key
std::vector<int>::iterator OA_hash::find(int key) {
    int code = hasher(key); //get the hash code
    std::vector<int>::iterator iter = table.begin() + code; //
    if(*iter == -1)  return table.end(); // the bucket was empty, return last
    if(*iter == key) return iter; // the key was found, return an iterator
    else { // the key was not in expected bucket, initiate probing
        for(iter; *iter != -1 && iter != table.end(); ++iter)
            if(*iter == key) return iter;
        for(iter = table.begin(); *iter != -1 && iter != table.begin()+code; ++iter)
            if(*iter == key) return iter;
        return table.end(); //the key was not found, return last
    }
}

//function to remove a key
void OA_hash::remove(int key) {
    auto i = find(key); // find the key
    *i = -2; // set bucket to removed
    --keys; // update key count
}

//function to rehash the table
void OA_hash::rehash(int n) {
    if(n <= bucket_count()) return; //do nothing
    std::list<int> temp; //create a temporary list to hold keys
    std::list<int>::const_iterator iter; //get a list iterator
    for(size_t i = 0; i < table.size(); ++i) {
        if(table[i] == -1) {} //skip empty buckets
        else temp.push_back(table[i]); //push all the keys into the list
    }
    table.resize(n); //update the table size
    for(int i = 0; i < n; ++i) //initialize all buckets to empty
        table[i] = -1;
    keys = 0;
    for(iter = temp.begin(); iter != temp.end(); ++iter)
        insert(*iter); //re-insert all of the keys
}

//function to display the hash table
void OA_hash::display() {
    int j = 0;
    for (auto i : table)
        std::cout << "\n[" << j++ << "," << i << "]";
    std::cout << '\n';
}
