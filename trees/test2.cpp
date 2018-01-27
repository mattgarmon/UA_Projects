//*****************************************************************************//
//  Matt Garmon ~ 2016                                                         //
//  test2.cpp - Secondary Test Suite for Various Types of Binary Trees         //
//*****************************************************************************//

#include <iostream>
#include <vector>
#include <ctime>
#include "BS_Tree.hpp"
#include "AVL_Tree.hpp"

std::mt19937 prbg; //pseudo-random bit generator

int main2(){

    //number of elements, max test elements, test increment, and random insert option
    int n, t, i;
    bool r;


    std::cout << "Max elements? ";
    std::cin >> t;

    std::cout << "Increment? ";
    std::cin >> i;

    std::cout << "Random(1) or Incremental(0) insert? ";
    std::cin >> r;

    //run the test a number of times
    for(n = i; n <= t; n += i) {

        //get an incremental/randomized vector of size n
        std::vector<int> test(n);
        std::iota(std::begin(test), std::end(test), 0);
        if (r) std::shuffle(begin(test), end(test), prbg);

        /*uncomment for binary search tree*/
        BS_Tree<int> m;

        /*uncomment for AVL tree
        AVL_Tree<int> m;*/

        //insert all of the elements
        /*uncomment for timed insert
        auto ibs_start = clock();*/
        for(auto i : test){
            m.insert(i);
        }
        /*auto ibs_stop = clock();
        std::cout << (ibs_stop - ibs_start)/double(CLOCKS_PER_SEC)*1000 << "\n";*/

        /*uncomment for max depth measurement
        std::cout << m.max_depth(m.get_root()) << "\n";*/

        //shuffle the element vector for randomly finding and erasing
        std::shuffle(begin(test), end(test), prbg);

        //randomly find all the elements
        /*uncomment for timed find
        auto ibs_start = clock();
        for(auto i : test){
            m.find(i);
        }
        auto ibs_stop = clock();
        std::cout << (ibs_stop - ibs_start)/double(CLOCKS_PER_SEC)*1000 << "\n";*/

        //randomly find and erase all the elements
        /*uncomment for timed erase
        auto ibs_start = clock();
        for(auto i : test){
            m.erase(m.find(i));
        }
        auto ibs_stop = clock();
        std::cout << (ibs_stop - ibs_start)/double(CLOCKS_PER_SEC)*1000 << "\n";*/
    }

return 0;

}

