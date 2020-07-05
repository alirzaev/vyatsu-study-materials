#include <fstream>
#include <random>
#include <set>
#include <algorithm>

int randrange(int a, int b) {
    static std::default_random_engine engine;

    std::uniform_int_distribution<int> dist(a, b - 1);

    return dist(engine);
}

void generate(std::ostream& output, int vert_count, int edges_count) {
    std::set<std::pair<int, int>> edges;
    while (edges.size() < edges_count) {
        int a = randrange(0, vert_count);
        int b = randrange(0, vert_count);
        if (a != b) {
            edges.insert(std::minmax(a, b));
        }
    }

    output << vert_count << ' ' << edges_count << std::endl;
    for (const auto& edge : edges) {
        output << edge.first << ' ' << edge.second << std::endl;
    }
}

int main() {
    std::ofstream output("../input.txt");

    std::pair<int, int> params[] = {
            {75, 1000},
            {75, 2500}, {90, 300}, {95, 2500}, {150, 6500},
            {170, 10000}, {250, 14000}, {250, 17000}, {300, 34000}
    };
/*    std::pair<int, int> params[] = {
//            {200, 15000},
//            {550, 113437},
//            {750, 168750},
//            {930, 324337},
//            {1250, 585937},
//            {1420, 756150},
            {1680, 1058400},
            {2568, 2472984},
            {3782, 5363821}
    };*/

    constexpr size_t cases_count = std::size(params);

    output << cases_count << std::endl;
    for (int i = 0; i < cases_count; ++i) {
        generate(output, params[i].first, params[i].second);
        output << std::endl;
    }

    return 0;
}