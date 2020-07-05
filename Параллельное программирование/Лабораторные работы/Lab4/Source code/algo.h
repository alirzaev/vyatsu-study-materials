//
// Created by ali on 25.04.18.
//

#ifndef LAB4_ALGO_H
#define LAB4_ALGO_H


#include <omp.h>
#include <vector>
#include <iostream>
#include <cstdint>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <cstring>
#include <random>
#include <chrono>
#include <numeric>
#include <bitset>

using namespace std::chrono;

using graph_t = std::vector<std::vector<size_t>>;

std::istream& operator>>(std::istream& is, graph_t& graph) {
    size_t n; is >> n; // vertexes
    size_t m; is >> m; // edges
    graph.clear();
    graph.resize(n);

    for (size_t i = 0; i < m; ++i) {
        size_t a, b; is >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    return is;
}

namespace improved {

    size_t _colorize(const graph_t &, const std::vector<size_t> &);

    size_t colorize(const graph_t &graph, size_t perm_count) {
        std::vector<size_t> order(graph.size());
        std::iota(order.begin(), order.end(), 0);

        std::vector<std::vector<size_t>> perms(perm_count);

        std::mt19937 g(static_cast<unsigned int>(time(nullptr)));
        for (size_t i = 0; i < perm_count; ++i) {
            std::shuffle(order.begin(), order.end(), g);
            perms[i] = order;
        }

        std::vector<size_t> results(perm_count, graph.size());

        for (size_t i = 0; i < perm_count; i++) {
            results[i] = _colorize(graph, perms[i]);
        }

        return *std::min_element(results.begin(), results.end());
    }

    inline size_t _mex(const std::vector<size_t> &set) {
        return static_cast<size_t>(std::find(set.begin(), set.end(), 0) - set.begin());
    }

    size_t _colorize(const graph_t &graph, const std::vector<size_t> &order) {
        size_t size = graph.size();

        std::vector<size_t> colored(size, 0);
        std::vector<size_t> colors(size, 0);
        std::vector<size_t> used_colors(size, 0);

        for (size_t v : order) {
            if (!colored[v]) {
                for (auto to : graph[v]) {
                    if (colored[to]) {
                        used_colors[colors[to]] = 1;
                    }
                }
                colored[v] = 1;

                auto color = _mex(used_colors);
                colors[v] = color;
                used_colors.assign(size, 0);
            }
        }

        return 1 + *std::max_element(colors.begin(), colors.end());
    }

}


#endif //LAB4_ALGO_H
