#pragma once

#include <stdlib.h>

#define EDGES_LIMIT(nvert) ((size_t)(nvert * nvert * 0.375))

typedef unsigned short int ushort_t;

size_t colorize(ushort_t *graph, size_t nvert, size_t nperms);

ushort_t *generate(size_t nvert, size_t nedges) __attribute__ ((warn_unused_result));
