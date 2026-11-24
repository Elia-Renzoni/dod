#include <chrono>
#include <cstdio>
#include <string>
#include <vector>

struct Entity {
    int position;
    std::string name;
};

struct EntityList {
    std::vector<Entity> entities;

    void push(int position, std::string name) {
        entities.push_back(Entity{
            .position = position,
            .name = std::move(name),
        });
    }

    std::vector<Entity>::iterator begin() { return entities.begin(); }
    std::vector<Entity>::iterator end() { return entities.end(); }
};

EntityList loadData(int n) {
    EntityList list;
    list.entities.reserve(n);
    for (int i = 0; i < n; ++i) {
        list.push(i, "entity_" + std::to_string(i));
    }
    return list;
}

int main() {
    const int N = 10'000'000;
    EntityList list = loadData(N);

    std::printf("sizeof(Entity) = %zu byte\n", sizeof(Entity));

    auto t0 = std::chrono::steady_clock::now();

    long long sum = 0;
    for (auto& e : list) {
        sum += e.position;
    }

    auto t1 = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    std::printf("sum = %lld\n", sum);
    std::printf("bad  (vector di struct): %.2f ms\n", ms);

    return 0;
}
