//
// Created by Али on 27.02.2018.
//

#ifndef LAB2_ALGO_H
#define LAB2_ALGO_H


#include <vector>
#include <iostream>
#include <cstdint>
#include <set>
#include <algorithm>
#include <thread>
#include <random>

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

std::ostream& operator<<(std::ostream& os, const graph_t& graph) {
    std::set<std::pair<size_t, size_t>> edges;

    for (size_t v = 0; v < graph.size(); ++v) {
        for (size_t to : graph[v]) {
            edges.emplace(std::minmax(v, to));
        }
    }

    os << graph.size() << ' ' << edges.size() << std::endl;
    for (const auto& p : edges) {
        os << p.first << ' ' << p.second << std::endl;
    }

    return os;
}

namespace improved {

    size_t _colorize(const graph_t&, const std::vector<size_t>&);

    size_t colorize(const graph_t& graph) {
        size_t orders_count = 500;

        const size_t thread_count = 4;
        size_t results[thread_count] = { 0 };

        auto routine = [&graph](size_t* r, size_t first, size_t last) {
            std::vector<size_t> order(graph.size());
            for (size_t i = 0; i < graph.size(); ++i) {
                order[i] = i;
            }
            auto rnd = std::mt19937{std::random_device{}()};
            size_t min = graph.size();
            for (size_t i = first; i < last; ++i) {
                std::shuffle(order.begin(), order.end(), rnd);
                min = std::min(min, _colorize(graph, order));
            }
            *r = min;
        };
        // run graph coloring
        std::thread threads[thread_count];
        for (size_t i = 0; i < thread_count; ++i) {
            size_t len = orders_count / thread_count;
            threads[i] = std::thread(routine, results + i, i * len, (i + 1) * len);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        return *std::min_element(results, results + thread_count);
    }

    bool _check_coloring(const graph_t& graph, const std::vector<size_t>& coloring) {
        for (int v = 0; v < graph.size(); ++v) {
            for (size_t to : graph[v]) {
                if (coloring[v] == coloring[to]) {
                    return false;
                }
            }
        }
        return true;
    }
/*
    size_t _mex(const std::set<size_t>& set) {
        if (set.empty()) {
            return 0;
        }

        auto max = *set.rbegin();

        for (size_t c = 0; c <= max + 1; ++c) {
            if (set.find(c) == set.end()) {
                return c;
            }
        }
    }

    size_t _colorize(const graph_t& graph, const std::vector<size_t>& order) {
        size_t size = graph.size();

        std::vector<size_t> colored(size, 0);
        std::vector<size_t> colors(size, 0);

        for (size_t i = 0; i < size; ++i) {
            size_t v = order[i];
            if (!colored[v]) {
                std::set<size_t> used_colors;
                for (auto to : graph[v]) {
                    if (colored[to]) {
                        used_colors.emplace(colors[to]);
                    }
                }

                auto color = _mex(used_colors);
                colored[v] = 1;
                colors[v] = color;
            }
        }

        if (_check_coloring(graph, colors)) {
            return 1 + *std::max_element(colors.begin(), colors.end());
        } else {
            return graph.size();
        }
    }
*/
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

#endif //LAB2_ALGO_H
