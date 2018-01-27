//*****************************************************************************//
//  Matt Garmon ~ 2016                                                         //
//  Test.cpp -                 //
//*****************************************************************************//

#include "SC_hash.hpp"
#include "OA_hash.hpp"
#include <ctime>
#include <iomanip>


int main() {

int s, n, it, rng;

while(true) {
        std::mt19937 prbg;
        std::cout << "Initial number of buckets? ";
        std::cin >> s;

        SC_hash sc(s);
        OA_hash oa(s);

        std::cout << "How many keys? ";
        std::cin >> n;

        std::vector<int> v(n);

        std::cout << "Increasing insert(1) or Random insert(2)? ";
        std::cin >> it;

        if(it == 2) {
            std::cout << "Range of keys? 0 to ";
            std::cin >> rng;
            std::uniform_int_distribution<int> dist(1, rng);
            auto gen = std::bind(dist, prbg);
            generate(begin(v), end(v), gen);
        }
        else if(it == 1) {
            std::iota(std::begin(v), std::end(v), 0);
        }
        std::cout << "|=========================================================|\n";
        std::cout << "|        Property         |      SC       |      OA       |\n";
        std::cout << "|---------------------------------------------------------|\n";


        std::cout << " Inserting elements:";
        auto ibs_start = clock();
        for (auto i : v) {
            sc.insert(i);
        }
        auto ibs_stop = clock();
        std::cout << std::right << std::setw(19) << (ibs_stop - ibs_start)/double(CLOCKS_PER_SEC)*1000 << " ms";
        auto iavl_start = clock();
        for (auto i : v) {
            oa.insert(i);
        }
        auto iavl_stop = clock();
        std::cout << std::right << std::setw(13) << (iavl_stop - iavl_start)/double(CLOCKS_PER_SEC)*1000 << " ms\n";

        std::iota(std::begin(v), std::end(v), n/10);
        std::shuffle(std::begin(v), std::end(v), prbg);


        std::cout << " Randomly finding elements:";
        auto fbs_start = clock();
        for (auto i : v) {
            sc.find(i);
        }
        auto fbs_stop = clock();
        std::cout << std::right << std::setw(12) << (fbs_stop - fbs_start)/double(CLOCKS_PER_SEC)*1000 << " ms";
        auto favl_start = clock();
        for (auto i : v) {
            oa.find(i);
        }
        auto favl_stop = clock();
        std::cout << std::right << std::setw(13) << (favl_stop - favl_start)/double(CLOCKS_PER_SEC)*1000 << " ms\n";


        std::shuffle(begin(v), end(v), prbg);

        std::cout << " Randomly erasing elements:";
        auto ebs_start = clock();
        for (auto i : v) {
            sc.remove(*sc.find(i));
        }
        auto ebs_stop = clock();
        std::cout << std::right << std::setw(12) << (ebs_stop - ebs_start)/double(CLOCKS_PER_SEC)*1000 << " ms";

        auto eavl_start = clock();
        for (auto i : v) {
            oa.remove(*oa.find(i));
        }
        auto eavl_stop = clock();
        std::cout << std::right << std::setw(13) << (eavl_stop - eavl_start)/double(CLOCKS_PER_SEC)*1000 << " ms\n";
        std::cout << "|=========================================================|\n\n";

}

return 0;
}

