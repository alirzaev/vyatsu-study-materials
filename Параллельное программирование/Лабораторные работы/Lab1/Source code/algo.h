#ifndef LAB1_ALGO_H
#define LAB1_ALGO_H

#include <vector>
#include <iostream>
#include <cstdint>
#include <set>
#include <unordered_set>
#include <algorithm>

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

bool check_coloring(const graph_t& graph, const std::vector<size_t>& coloring) {
    for (int v = 0; v < graph.size(); ++v) {
        for (size_t to : graph[v]) {
            if (coloring[v] == coloring[to]) {
                return false;
            }
        }
    }
    return true;
}

namespace improved {

    size_t _mex(const std::unordered_set<size_t>& set) {
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

    size_t _mex(const std::vector<size_t> &set) {
        return static_cast<size_t>(std::find(set.begin(), set.end(), 0) - set.begin());
    }

    size_t colorize(const graph_t& graph) {
        size_t size = graph.size();
        std::vector<size_t> order(size);

        size_t orders_count = 500;
        std::vector<std::vector<size_t>> orders(orders_count);

        for (size_t i = 0; i < size; ++i) {
            order[i] = i;
        }

        for (auto& ord : orders) {
            std::random_shuffle(order.begin(), order.end());
            ord = order;
        }

        size_t min = graph.size();

        for (const auto& ord : orders) {
            std::vector<size_t> colored(size, 0);
            std::vector<size_t> colors(size, 0);

//            std::unordered_set<size_t> used_colors;
//            used_colors.reserve(size);
            std::vector<size_t> used_colors(size, 0);

            for (size_t v : ord) {
                if (!colored[v]) {
                    for (auto to : graph[v]) {
                        if (colored[to]) {
//                            used_colors.emplace(colors[to]);
                            used_colors[colors[to]] = 1;
                        }
                    }

                    auto c = _mex(used_colors);
                    colored[v] = 1;
                    colors[v] = c;

//                    used_colors.clear();
                    used_colors.assign(size, 0);
                }
            }
            size_t colors_count = 1 + *std::max_element(colors.begin(), colors.end());
            min = std::min(min, colors_count);
        }

        return min;
    }

}

#endif //LAB1_ALGO_H
