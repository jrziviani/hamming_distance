#ifndef BASE_H
#define BASE_H

namespace waes
{
    // defines how many test cases will be created
    const uint32_t MAX_N_TESTS = 10000;

#if __x86_64__
    using unsigned_t = uint64_t;
#else
    using unsigned_t = uint32_t;
#endif
}

#endif // BASE_H
