//*****************************************************************************//
//  Matt Garmon ~ 2016                                                         //
//  test.cpp - Test Suite for Various Types of Binary Trees                    //
//*****************************************************************************//

#include <iostream>
#include <random>
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <ctime>
#include <iomanip>
#include <memory>
#include <cstdint>
#include "BS_Tree.hpp"
#include "AVL_Tree.hpp"

int main() {

    int n, it, rng;
    BS_Tree<int> bs;
    AVL_Tree<int> avl;
    std::mt19937 prbg;


    while(true) {
            std::cout << "How many elements? ";
            std::cin >> n;

            std::vector<int> v(n);

            std::cout << "Increasing insert (1) or Random insert (2)? ";
            std::cin >> it;

            if(it == 2) {
                std::cout << "Range = 0 to? ";
                std::cin >> rng;
                std::uniform_int_distribution<int> dist(1, rng);
                auto gen = std::bind(dist, prbg);
                generate(begin(v), end(v), gen);
            }
            else if(it == 1) {
                std::iota(std::begin(v), std::end(v), 0);
            }
            std::cout << "|=========================================================|\n";
            std::cout << "|        Property         |      BS       |      AVL      |\n";
            std::cout << "|---------------------------------------------------------|\n";


            std::cout << " Inserting elements:";
            auto ibs_start = clock();
            for (auto i : v) {
                bs.insert(i);
            }
            auto ibs_stop = clock();
            std::cout << std::right << std::setw(19) << (ibs_stop - ibs_start)/double(CLOCKS_PER_SEC)*1000 << " ms";
            auto iavl_start = clock();
            for (auto i : v) {
                avl.insert(i);
            }
            auto iavl_stop = clock();
            std::cout << std::right << std::setw(13) << (iavl_stop - iavl_start)/double(CLOCKS_PER_SEC)*1000 << " ms\n";

            int bs_depth = bs.max_depth(bs.get_root());
            int avl_depth = avl.max_depth(avl.get_root());
            std::cout << " Max Height:";
            std::cout << std::right << std::setw(30) << bs_depth;
            std::cout << std::right << std::setw(16) << avl_depth << "\n";

            std::shuffle(begin(v), end(v), prbg);

            std::cout << " Randomly finding elements:";
            auto fbs_start = clock();
            for (auto i : v) {
                bs.find(i);
            }
            auto fbs_stop = clock();
            std::cout << std::right << std::setw(12) << (fbs_stop - fbs_start)/double(CLOCKS_PER_SEC)*1000 << " ms";
            auto favl_start = clock();
            for (auto i : v) {
                avl.find(i);
            }
            auto favl_stop = clock();
            std::cout << std::right << std::setw(13) << (favl_stop - favl_start)/double(CLOCKS_PER_SEC)*1000 << " ms\n";


            std::shuffle(begin(v), end(v), prbg);

            std::cout << " Randomly erasing elements:";
            auto ebs_start = clock();
            for (auto i : v) {
                bs.erase(bs.find(i));
            }
            auto ebs_stop = clock();
            std::cout << std::right << std::setw(12) << (ebs_stop - ebs_start)/double(CLOCKS_PER_SEC)*1000 << " ms";

            auto eavl_start = clock();
            for (auto i : v) {
                avl.erase(avl.find(i));
            }
            auto eavl_stop = clock();
            std::cout << std::right << std::setw(13) << (eavl_stop - eavl_start)/double(CLOCKS_PER_SEC)*1000 << " ms\n";
            std::cout << "|=========================================================|\n\n";
    }

    return 0;
}
