#ifndef HAMMING_DISTANCE_H
#define HAMMING_DISTANCE_H

#include "base.h"

namespace waes
{
    template <class Derived>
    class string_distance
    {
        public:
        unsigned_t distance(unsigned_t a, unsigned_t b) const
        {
            return static_cast<const Derived *const>(this)->distance(a, b);
        }
    };

    class hamming : public string_distance<hamming>
    {
        public:
        unsigned_t distance(unsigned_t a, unsigned_t b) const;
    };

    class loop_hamming : public string_distance<loop_hamming>
    {
        public:
        unsigned_t distance(unsigned_t a, unsigned_t b) const;
    };
}

extern "C"
{
    waes::unsigned_t binary_distance(waes::unsigned_t a, waes::unsigned_t b);
}

#endif // HAMMING_DISTANCE_H
