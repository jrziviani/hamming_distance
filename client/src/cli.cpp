#include <iostream>
#include <cstdint>
#include <dlfcn.h>
#include <string>

using namespace std;

class client
{
    using handler_t = void*;

    handler_t handler_;

private:
    uint64_t hamming_distance(uint64_t a, uint64_t b)
    {
        using fn_t = uint64_t (*)(uint64_t, uint64_t);
        auto fn = reinterpret_cast<fn_t>(::dlsym(handler_, "binary_distance"));
        return fn(a, b);
    }

public:
    client()
    {
        handler_ = ::dlopen("libhamming.so", RTLD_LAZY);
        if (handler_ == nullptr) {
            cerr << "Impossible to load hamming_distance.so, exiting\n";
            exit(1);
        }
    }

    uint64_t hamming_distance(string a, string b)
    {
        if (a.size() != b.size()) {
            cerr << "both strings must have the same size, exiting\n";
            exit(2);
        }

        uint64_t distance = 0;
        for (size_t i = 0; i < a.size(); i++) {
            if (hamming_distance(a[i], b[i]) != 0)
                distance++;
        }

        return distance;
    }

    ~client()
    {
        ::dlclose(handler_);
    }
};

void help(const char *name)
{
    cout << "Usage:\n"
         << "  ./" << name << " <string> <string>\n"
         << "NOTE: use quotes to include empty spaces\n";
}

int main(int argc, char *argv[])
{
    client cli;
    if (argc != 3) {
        help(argv[0]);
        return 3;
    }

    cout << cli.hamming_distance(argv[1], argv[2]) << '\n';

    return 0;
}
