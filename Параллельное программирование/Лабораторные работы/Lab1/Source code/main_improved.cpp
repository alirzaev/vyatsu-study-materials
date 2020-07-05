#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

#include "algo.h"

using namespace std::chrono;

int main() {
    std::ifstream input("../input.txt");
    std::ofstream output("../output.txt");

    graph_t graph;
    size_t cnt;
    input >> cnt;
    output << cnt << std::endl;
    for (size_t i = 0; i < cnt; ++i) {
        input >> graph;
        auto start = std::chrono::system_clock::now();

        auto res = improved::colorize(graph);

        auto stop = std::chrono::system_clock::now();

        auto time = duration_cast<milliseconds>(stop - start).count();

//        output << std::setw(3) << graph.size() << ' '
//               << std::setw(4) << res << ' '
//               << time << std::endl;
        output << res << std::endl;
        std::cout << time << std::endl;
    }

    return 0;
}