(c) Matt Garmon - 2016

An implementation of the hash table data structure. Includes two seperate versions for dealing with
collisions. The first being seperate chaining, which makes each bucket a linked list to search through
when finding an element. The second being open addressing, which will iterate over the table until an 
empty bucket is discovered.

Includes testing suite to demonstrate performance differences.