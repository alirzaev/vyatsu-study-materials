//
// Created by Али on 26.03.2018.
//

#ifndef LAB3_ALGO_H
#define LAB3_ALGO_H

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

std::istream &operator>>(std::istream &is, graph_t &graph) {
    size_t n;
    is >> n; // vertexes
    size_t m;
    is >> m; // edges
    graph.clear();
    graph.resize(n);

    for (size_t i = 0; i < m; ++i) {
        size_t a, b;
        is >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    return is;
}

std::ostream &operator<<(std::ostream &os, const graph_t &graph) {
    std::set<std::pair<size_t, size_t>> edges;

    for (size_t v = 0; v < graph.size(); ++v) {
        for (size_t to : graph[v]) {
            edges.emplace(std::minmax(v, to));
        }
    }

    os << graph.size() << ' ' << edges.size() << std::endl;
    for (const auto &p : edges) {
        os << p.first << ' ' << p.second << std::endl;
    }

    return os;
}

namespace improved {

    constexpr size_t _thread_count = 3;

    size_t _colorize(const graph_t &, const std::vector<size_t> &);

    size_t colorize(const graph_t &graph) {
        constexpr size_t perm_count = 500;

        std::vector<size_t> order(graph.size());
        std::iota(order.begin(), order.end(), 0);

        std::vector<std::vector<size_t>> perms(perm_count);

        std::mt19937 g(static_cast<unsigned int>(time(nullptr)));
        for (size_t i = 0; i < perm_count; ++i) {
            std::shuffle(order.begin(), order.end(), g);
            perms[i] = order;
        }

        std::vector<size_t> results(perm_count, graph.size());

#pragma omp parallel for num_threads(_thread_count) schedule(dynamic, 166)
        for (size_t i = 0; i < perm_count; i++) {
            results[i] = _colorize(graph, perms[i]);
        }

        return *std::min_element(results.begin(), results.end());
    }

    bool _check_coloring(const graph_t &graph, const std::vector<size_t> &coloring) {
        for (size_t v = 0; v < graph.size(); ++v) {
            for (size_t i = 0; i < graph[v].size(); ++i) {
                size_t to = graph[v][i];
                if (coloring[v] == coloring[to]) {
                    return false;
                }
            }
        }
        return true;
    }

    size_t _mex(const std::unordered_set<size_t> &set) {
        if (set.empty()) {
            return 0;
        }

        size_t max = *std::max_element(set.begin(), set.end());

        for (size_t c = 0; c <= max + 1; ++c) {
            if (set.find(c) == set.end()) {
                return c;
            }
        }
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

    size_t _colorize_(const graph_t &graph, const std::vector<size_t> &order) {
        size_t size = graph.size();

        std::vector<size_t> colored(size, 0);
        std::vector<size_t> colors(size, 0);

        std::unordered_set<size_t> used_colors;
        used_colors.reserve(size);

        for (size_t v : order) {
            if (!colored[v]) {
                for (auto to : graph[v]) {
                    if (colored[to]) {
                        used_colors.emplace(colors[to]);
                    }
                }
                colored[v] = 1;

                auto color = _mex(used_colors);
                colors[v] = color;

                used_colors.clear();
            }
        }

        return 1 + *std::max_element(colors.begin(), colors.end());
    }

}

#endif //LAB3_ALGO_H
