#include <chrono>
#include <cstdio>
#include <string>
#include <vector>

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

long long compute(bool linearly, EntityList& list, int target) {
    if (linearly) {
        return compLinear(list, target);
    }
    return compBinary(list, target);
}

int main(int argc, char* argv[]) {
    bool linearly = true;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--binary" || arg == "-b") {
            linearly = false;
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

    const int target = N - 1;  

    auto t0 = std::chrono::steady_clock::now();
    long long idx = compute(linearly, list, target);
    auto t1 = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    std::printf("target = %d -> index = %lld\n", target, idx);
    std::printf("good (two vectors):       %.4f ms\n", ms);

    return 0;
}
