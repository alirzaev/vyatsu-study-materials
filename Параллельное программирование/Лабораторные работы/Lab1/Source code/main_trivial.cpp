#include <iostream>
#include <chrono>

#include "algo.h"

using namespace std::chrono;

int main() {
    freopen("../input.txt", "r", stdin);
    freopen("../output.txt", "w", stdout);

    graph_t graph;
    size_t cnt;
    std::cin >> cnt;
    for (size_t i = 0; i < cnt; ++i) {
        std::cin >> graph;
        auto start = std::chrono::system_clock::now();

        auto number = trivial::colorize(graph);

        auto stop = std::chrono::system_clock::now();

        auto time = duration_cast<milliseconds>(stop - start).count();
        std::cout << number << " " << time << std::endl;
    }

    return 0;
}