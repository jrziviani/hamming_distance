#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <array>
#include <chrono>

#include "hamming_distance.h"
#include "base.h"

namespace waes_test
{
    using namespace std;
    using namespace waes;

    class tester
    {
        loop_hamming lhm_;
        hamming hm_;

    private:
        int run_expected(unsigned_t a, unsigned_t b, int expected)
        {
            int code = 0;
            auto ra = lhm_.distance(a, b);
            if (ra != expected) {
                cerr << "loop hamming error: " << a << ", " << b
                     << " : expected " << expected << ", got: " << ra << "\n";
                code = 1;
            }

            ra = hm_.distance(a, b);
            if (ra != expected) {
                cerr << "hamming error: " << a << ", " << b
                     << " : expected " << expected << ", got: " << ra << "\n";
                code = 1;
            }

            return code;
        }

        int run_hm(unsigned_t a, unsigned_t b)
        {
            return hm_.distance(a, b);
        }

        int run_lhm(unsigned_t a, unsigned_t b)
        {
            return lhm_.distance(a, b);
        }

    public:
        tester()
        {
        }

        void run_wired_cases()
        {
            bool pass = true;
            // this function tries to identify and stress some possible corner
            // cases in order to understand how the library will behave under
            // such circunstances

            // NEGATIVE TEST
            // -1 will converted to the max value allowed to be stored in a
            // 32-bit or 64-bit storage, in other words it will be
            // 11111111111111111111111111111111 vs
            // 00000000000000000000000000000000
#if __x86_64__
            if (run_expected(-1, 0, 64) != 0)
#else
            if (run_expected(-1, 0, 32) != 0)
#endif
                pass = false;

            // OVERFLOW TEST
            // there's no overflow with unsigned types, this test will
            // basically round around the maximum value to the minimum value,
            // in other words: max(unsigned) + 1 == 0
            //                 max(unsigned) + 2 == 1
            if (run_expected(numeric_limits<unsigned_t>::max() + 1,
                             numeric_limits<unsigned_t>::max() + 2,
                             1) != 0)
                pass = false;

            // FLOATING POINTS
            // floating point values will be trunc'ed to regular unsigned
            // integers, ie: 14.32 => 14, 12.55 => 12
            if (run_expected(14.32, 12.55, 1) != 0)
                pass = false;

            // CHARACTERS
            // well, it's basically a representantion of a number get from the
            // ascii table. This algorithm is binary aware, so it converts
            // 'a' => 97, 'z' => 122
            // then check the distance using
            //  97 => 1100001
            // 122 => 1111010
            //          ^^ ^^ -> 4 differences
            if (run_expected('a', 'z', 4) != 0)
                pass = false;

            // print user frindly messages indicating the test status
            if (pass) {
                cout << "[ OK ] 4 test cases run successfuly.\n";
            }
            else {
                cout << "[ FAIL ] check log above for further information.\n";
            }
        }

        void run_random_cases()
        {
            // open the test case file for reading, please make sure this program
            // has permission rights to do it.
            ifstream test_file("hamming_test_case.txt");
            if (test_file.fail()) {
                cerr << "Cannot open hamming_test_case.txt for reading, "
                     << "exiting!\n";
                exit(1);
            }

            unsigned_t a;
            unsigned_t b;
            int distance;
            bool pass = true;
            uint32_t line = 1;

            // run each test case by using both algorithms and make sure they
            // return the expected result
            while (test_file >> a >> b >> distance) {
                line++;
                if (run_expected(a, b, distance) != 0)
                    pass = false;
            }
            test_file.close();

            // print user frindly messages indicating the test status
            if (pass) {
                cout << "[ OK ] " << line << " test cases run successfuly.\n";
            }
            else {
                cout << "[ FAIL ] check log above for further information.\n";
            }
        }

        void run_speed_cases()
        {
            using namespace chrono;

            // this test intends to find the fastest hamming function
            // each case will run 4 times, the mean will be considered.

            // open the test case file for reading, please make sure this program
            // has permission rights to do it.
            ifstream test_file("hamming_test_case.txt");
            if (test_file.fail()) {
                cerr << "Cannot open hamming_test_case.txt for reading, "
                     << "exiting!\n";
                exit(1);
            }

            // store each test case in contiguous memory before running the
            // algorithms
            unsigned_t a;
            unsigned_t b;
            int ignore;
            array<unsigned_t, MAX_N_TESTS * 2> arr;
            size_t idx = 0;
            while (test_file >> a >> b >> ignore) {
                arr[idx++] = a;
                arr[idx++] = b;
            }
            test_file.close();

            double result = 0.0;
            for (size_t t = 1; t <= MAX_N_TESTS; t++) {
                auto start = high_resolution_clock::now();
                for (size_t i = 0; i < 2000; i += 2) {
                    run_hm(arr[i], arr[i + 1]);
                }
                result += duration_cast<microseconds>(
                        high_resolution_clock::now() - start).count();
            }
            cout << "[ OK ] hamming distances takes " << result / MAX_N_TESTS
                 << " microseconds to run the whole test case file\n";

            double result2 = 0.0;
            for (size_t t = 1; t <= MAX_N_TESTS; t++) {
                auto start = high_resolution_clock::now();
                for (size_t i = 0; i < 2000; i += 2) {
                    run_lhm(arr[i], arr[i + 1]);
                }
                result2 += duration_cast<microseconds>(
                        high_resolution_clock::now() - start).count();
            }
            cout << "[ OK ] loop_hamming distances takes "
                 << result2 / MAX_N_TESTS
                 << " microseconds to run the whole test case file\n";

            cout << "[ OK ] first algorithm version is "
                 << (1 - (result / result2)) * 100
                 << "\% faster\n";
        }
    };
}

int main()
{
    // test case main execution point
    waes_test::tester tester;
    tester.run_random_cases();
    tester.run_wired_cases();
    tester.run_speed_cases();

    return 0;
}
