#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <mpi.h>

#include "algo.h"

/* Number of permutations */
#define NPERMS 500u

/* Number of vertices (default value) */
#define NVERT 1500u

#define MASTER_RANK 0

void master_routine(int ws, size_t nvert) {
    size_t result, g_time;
    ushort_t *graph;
    double g_start, g_stop;

    printf("Number of vertices: %lu\n", nvert);

    printf("Generating graph... ");
    graph = generate(nvert, EDGES_LIMIT(nvert));
    printf("OK\n");

    g_start = MPI_Wtime() * 1000.0;

    printf("Sending data to workers... ");
    for (int rank = 1; rank < ws; ++rank) {
        MPI_Send(graph, 1, MPI_UNSIGNED_SHORT, rank, 0, MPI_COMM_WORLD);
        MPI_Send(graph + 1, graph[0] * graph[0], MPI_UNSIGNED_SHORT, rank, 0, MPI_COMM_WORLD);
    }
    printf("OK\n");

    printf("Solving... ");
    result = colorize(graph, nvert, NPERMS / ws);

    for (int rank = 1; rank < ws; ++rank) {
        size_t r;
        MPI_Recv(&r, 1, MPI_UNSIGNED_LONG, rank, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        result = result > r ? r : result;
    }
    printf("OK\n");

    g_stop = MPI_Wtime() * 1000.0;
    g_time = (size_t)(g_stop - g_start);

    printf("Result: %4lu\n", result);
    printf("Global time spent: %lu msec.\n", g_time);

    printf("Shutting down workers... ");
    for (int rank = 1; rank < ws; ++rank) {
        /* (0) is terminating message */
        ushort_t buf = 0;
        MPI_Send(&buf, 1, MPI_UNSIGNED_SHORT, rank, 0, MPI_COMM_WORLD);
    }
    printf("OK\n");

    free(graph);
}

void slave_routine(int wr, int ws) {
    while (1) {
        size_t nperms, result, nvert_ul;
        ushort_t nvert;
        ushort_t *graph;

        nperms = NPERMS / ws;

        /* Receive data: number of vertices */
        MPI_Recv(&nvert, 1, MPI_UNSIGNED_SHORT, MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        /* (0) is terminating message */
        if (nvert == 0) {
            return;
        }

        nvert_ul = nvert;

        graph = (ushort_t *) calloc(nvert_ul * nvert_ul, sizeof(ushort_t));
        /* Receive data: graph matrix */
        MPI_Recv(graph, nvert_ul * nvert_ul, MPI_UNSIGNED_SHORT, MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        /* Solve */
        result = colorize(graph, nvert, nperms);

        /* Send results */
        MPI_Send(&result, 1, MPI_UNSIGNED_LONG, MASTER_RANK, 0, MPI_COMM_WORLD);

        free(graph);
    }
}

int main(int argc, char **argv) {
    int world_rank, world_size;
    size_t nvert;

    srand(time(NULL));

    MPI_Init(&argc, &argv);

    /* Pass the number of vertices as CLI argument */
    if (argc < 2 || (nvert = strtoul(argv[1], NULL, 10)) > USHRT_MAX || nvert == 0) {
        nvert = NVERT;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
        master_routine(world_size, nvert);
    } else {
        slave_routine(world_rank, world_size);
    }

    MPI_Finalize();

    return 0;
}
