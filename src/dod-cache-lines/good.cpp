#include <chrono>
#include <cstdio>
#include <string>
#include <vector>
#include <random>

struct EntityList {
    std::vector<int> positions;
    std::vector<std::string> names;

    void push(int position, std::string name) {
        positions.push_back(position);
        names.push_back(std::move(name));
    }
};

EntityList loadData(int n) {
    EntityList list;
    list.positions.reserve(n);
    list.names.reserve(n);
    for (int i = 0; i < n; ++i) {
        list.push(i, "entity_" + std::to_string(i));
    }
    return list;
}

long long compLinear(EntityList& list, int target) {
    for (size_t i = 0; i < list.positions.size(); ++i) {
        if (list.positions[i] == target) {
            return (long long)i;
        }
    }
    return -1;
}

long long compBinary(EntityList& list, int target) {
    size_t lo = 0;
    size_t hi = list.positions.size();   

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        int value = list.positions[mid];

        if (value == target) {
            return (long long)mid;
        }
        if (value < target) {
            lo = mid + 1;  
        } else {
            hi = mid;       
        }
    }
    return -1;
}

long long compBinaryPrefetch(EntityList& list, int target) {
    size_t lo = 0;
    size_t hi = list.positions.size();   
    int* data = list.positions.data();

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;

        __builtin_prefetch(data + lo + (mid - lo) / 2);
        __builtin_prefetch(data + mid + 1 + (hi - mid - 1) / 2);

        int value = list.positions[mid];

        if (value == target) {
            return (long long)mid;
        }
        if (value < target) {
            lo = mid + 1;  
        } else {
            hi = mid;       
        }
    }

    return -1;
}

long long compBinaryPrefetchBranchless(EntityList& list, int target) {
    const int* data = list.positions.data();
    const int* base = data;
    size_t n = list.positions.size();
    if (n == 0) return -1;

    while (n > 1) {
        size_t half = n / 2;
        size_t next = n - half;
        __builtin_prefetch(base + next / 2);
        __builtin_prefetch(base + half + next / 2);

        // Assembly: cmove, no branch needed!
        base = (base[half] <= target) ? base + half : base;
        n -= half;
    }
    return *base == target ? (long long)(base - data) : -1;
}

long long compute(bool linearly, EntityList& list, int target, bool prefetch, bool branchless) {
    if (linearly) {
        return compLinear(list, target);
    }

    if (prefetch && !branchless) {
        return compBinaryPrefetch(list, target);
    }

    if (branchless) {
        return compBinaryPrefetchBranchless(list, target);
    }

    return compBinary(list, target);
}

int main(int argc, char* argv[]) {
    bool linearly = true;
    bool cache_prefetch = false;
    bool branchless = false;
    std::string previousCmd;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (i > 1) {
            previousCmd = argv[i-1];
        }

        if (arg == "--binary" || arg == "-b") {
            linearly = false;
        } else if (arg == "--prefetch" && previousCmd == "--binary") {
            cache_prefetch = true;
        } else if (arg == "--branchless" && previousCmd == "--prefetch") {
            branchless = true;
        } else {
            std::fprintf(stderr, "Unknown: %s\n", argv[i]);
            std::fprintf(stderr, "Use: %s [--binary|-b]\n", argv[0]);
            return 1;
        }
    }

    const int N = 10'000'000;
    EntityList list = loadData(N);

    std::printf("sizeof(int) = %zu byte\n", sizeof(int));
    std::printf("mod': %s\n", linearly ? "linear" : "binary");

    const int Q = linearly? 20 : 2'000'000;
    std::mt19937 rng(42);                          
    std::uniform_int_distribution<int> dist(0, N - 1);
    std::vector<int> targets(Q);
    for (auto& t : targets) t = dist(rng);

    auto t0 = std::chrono::steady_clock::now();
    long long check = 0;
    for (int t : targets) {
        check += compute(linearly, list, t, cache_prefetch, branchless);
    }
    auto t1 = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    std::printf("check = %lld\n", check);
    std::printf("good (two vectors):       %.4f ms\n", ms);

    return 0;
}
