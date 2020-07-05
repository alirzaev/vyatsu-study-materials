#include <fstream>

#include "algo.h"

int main() {
    std::ifstream input("../output.txt");

    size_t n;
    input >> n;
    for (size_t i = 0; i < n; ++i) {
        graph_t graph;
        input >> graph;
        std::vector<size_t> colors(graph.size());

        for (auto &c : colors) {
            input >> c;
        }

        if (check_coloring(graph, colors)) {
            std::cout << "YES" << std::endl;
        } else {
            std::cout << "NO" << std::endl;
        }
    }
}