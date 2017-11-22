#include <random>
#include <cstdint>
#include <limits>
#include <fstream>
#include <iostream>

#include "base.h"

namespace waes_test
{
    using namespace std;
    using namespace waes;

    int hamming_distance(unsigned_t x, unsigned_t y)
    {
        // this function uses gcc built-in population count only for
        // test purposes, the results from this function will be used
        // as a template to check the results of the library.
#if __x86_64__
        return __builtin_popcountll(x ^ y);
#else
        return __builtin_popcount(x ^ y);
#endif
    }

    void generate_random_testcase()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<unsigned_t> r(0,
                numeric_limits<unsigned_t>::max());

        // open the test case file for writing, please make sure this program
        // has permission rights to do it.
        ofstream test_file("hamming_test_case.txt", ios::trunc);
        if (test_file.fail()) {
            cerr << "Cannot open hamming_test_case.txt for writing, exiting!\n";
            exit(1);
        }

        // generates a random 32-bit or 64-bit number, a variation of such
        // number, calculates the hamming distance between both numbers
        // using the template function and write it into the test case file.
        for (size_t i = 1; i < MAX_N_TESTS; i++) {
            unsigned_t a = r(gen);
            unsigned_t b = (r(gen) % 10) ? r(gen) : a - (a & i);
            test_file << a << " " << b << " " << hamming_distance(a, b) << '\n';
        }

        test_file.close();
    }
}

int main()
{
    waes_test::generate_random_testcase();
    return 0;
}
