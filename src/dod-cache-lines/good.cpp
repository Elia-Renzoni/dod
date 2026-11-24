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

int main() {
    const int N = 10'000'000;
    EntityList list = loadData(N);

    std::printf("sizeof(int) = %zu byte\n", sizeof(int));

    auto t0 = std::chrono::steady_clock::now();

    long long sum = 0;
    for (int p : list.positions) {
        sum += p;
    }

    auto t1 = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    std::printf("sum = %lld\n", sum);
    std::printf("good (due vector):       %.2f ms\n", ms);

    return 0;
}
