#include <iostream>

#include "hamming_distance.h"

namespace waes
{
    using namespace std;

    unsigned_t hamming::distance(unsigned_t a, unsigned_t b) const
    {
        // the fist step is to isolate any bit that is different in
        // comparison of a and b. Thanks to the "Exclusive OR" logic port,
        // this usually takes 1 instruction and turns any different bit, ie:
        // A   10010110
        // B   10010101
        // XOr 00000011
        unsigned_t dist = a ^ b;

        // than we only need to count how many "ones" were left as a result
        // of that XOr operation.
        // we could loop through each bit, masking, comparing and adding
        // to a counter variable, but another approach is to continue
        // using bitwise operators in order to avoid loops and branches in
        // a fashion known as population count using a bit more than 20
        // instructions.
#if __x86_64__
        dist = dist - ((dist >> 1) & 0x5555555555555555ULL);
        dist = (dist & 0x3333333333333333ULL) + ((dist >> 2) & 0x3333333333333333ULL);
        dist = (dist + (dist >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
        dist += (dist >> 8);
        dist += (dist >> 16);
        dist += (dist >> 32);

        return dist & 0x00000000000000FFULL;
#else
        dist = dist - ((dist >> 1) & 0x55555555);
        dist = (dist & 0x33333333) + ((dist >> 2) & 0x33333333);
        dist = (dist + (dist >> 4)) & 0x0F0F0F0F;
        dist += (dist >> 8);
        dist += (dist >> 16);

        return dist & 0x0000003F;
#endif
    }

    unsigned_t loop_hamming::distance(unsigned_t a, unsigned_t b) const
    {
        // the idea is the same, but using the classic algorithm to give us
        // parameters to compare both.
        unsigned_t dist = a ^ b;

        unsigned_t ret = 0;
        for (; dist > 0; dist &= (dist - 1), ret++) ;

        return ret;
    }
}

extern "C"
{
    waes::unsigned_t binary_distance(waes::unsigned_t a, waes::unsigned_t b)
    {
        waes::hamming hm;
        return hm.distance(a, b);
    }
}
