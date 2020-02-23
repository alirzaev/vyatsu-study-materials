#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "algo.h"

size_t _mex(size_t *data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == 0) {
            return i;
        }
    }

    return size;
}

size_t _max_element(size_t *data, size_t size) {
    size_t mx;

    mx = data[0];

    for (size_t i = 1; i < size; ++i) {
        if (data[i] > mx) {
            mx = data[i];
        }
    }

    return mx;
}

size_t _colorize(ushort_t *graph, size_t *order, size_t nvert) {
    size_t result;
    size_t *colored, *colors, *used_colors;

    colored = (size_t *) calloc(nvert, sizeof(size_t));
    colors = (size_t *) calloc(nvert, sizeof(size_t));
    used_colors = (size_t *) calloc(nvert, sizeof(size_t));

    for (size_t i = 0; i < nvert; ++i) {
        size_t v = order[i];

        if (!colored[v]) {
            for (size_t to = 0; to < nvert; ++to) {
                if (!graph[v * nvert + to]) {
                    continue;
                }

                if (colored[to]) {
                    used_colors[colors[to]] = 1;
                }
            }
            colored[v] = 1;

            size_t color = _mex(used_colors, nvert);
            colors[v] = color;
            memset(used_colors, 0, nvert * sizeof(size_t));
        }
    }

    result = 1 + _max_element(colors, nvert);

    free(used_colors);
    free(colors);
    free(colored);

    return result;
}

void _random_shuffle(size_t *data, size_t size) {
    ptrdiff_t i, n;

    n = (ptrdiff_t) size;
    for (i = n - 1; i > 0; --i) {
        size_t f = i, s = rand() % (i + 1);

        size_t t = data[f];
        data[f] = data[s];
        data[s] = t;
    }
}

size_t colorize(ushort_t *graph, size_t nvert, size_t nperms) {
    size_t *order;
    size_t result;

    order = (size_t *) calloc(nvert, sizeof(size_t));
    for (size_t i = 0; i < nvert; ++i) {
        order[i] = i;
    }

    result = SIZE_MAX;
    for (size_t i = 0; i < nperms; ++i) {
        _random_shuffle(order, nvert);
        size_t r = _colorize(graph, order, nvert);
        if (r < result) {
            result = r;
        }
    }

    free(order);

    return result;
}

ushort_t *generate(size_t nvert, size_t nedges) {
    ushort_t *graph, *matrix;
    size_t a, b;

    graph = (ushort_t *) calloc(1 + nvert * nvert, sizeof(ushort_t));
    graph[0] = (ushort_t) nvert;
    matrix = graph + 1;

    while (nedges) {
        a = rand() % nvert;
        b = rand() % nvert;

        if (a != b) {
            if (!matrix[a * nvert + b] && !matrix[b * nvert + a]) {
                matrix[a * nvert + b] = 1;
                matrix[b * nvert + a] = 1;
                nedges--;
            }
        }
    }

    return graph;
}
