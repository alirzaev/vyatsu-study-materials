#include <mpi.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <iomanip>
#include <vector>
#include <numeric>

#include "algo.h"

const int TASK_TAG = 0;
const int RES_TAG = 1;
const int TASKS_AMOUNT_TAG = 2;
const int MASTER_RANK = 0;

void master_routine(int ws) {
    std::ifstream input("../input.txt");
    std::ofstream output("../output.txt");

    size_t cnt;
    input >> cnt;
    output << cnt << std::endl;

    std::vector<size_t> results(cnt, std::numeric_limits<size_t>::max()), times(cnt, 0);

    for (size_t i = 0; i < cnt; ++i) {
        size_t rank_res[2];

        auto g_start = MPI_Wtime() * 1000.0;

        for (int rank = 1; rank < ws; ++rank) {
            MPI_Recv(&rank_res, 2, MPI_UNSIGNED_LONG, rank, static_cast<int>(i), MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            auto [time, res] = rank_res;

            results[i] = std::min(results[i], res);
            times[i] += time;
        }

        auto g_stop = MPI_Wtime() * 1000.0;
        auto g_time = static_cast<size_t>(g_stop - g_start);

        output << std::setw(4) << results[i] << ' '
               << g_time - times[i] / (ws - 1) << std::endl;
        std::cout << g_time << ' ' << times[i] / (ws - 1) << std::endl;
    }
}

void slave_routine(int wr, int ws) {
    size_t perm_count = 500u / (ws - 1);
    std::ifstream input("../input.txt");

    graph_t graph;
    size_t cnt;
    input >> cnt;

    for (size_t i = 0; i < cnt; ++i) {
        auto start = MPI_Wtime() * 1000.0;

        input >> graph;

        auto stop = MPI_Wtime() * 1000.0;
        auto time = static_cast<size_t>(stop-start);

        size_t result = improved::colorize(graph, perm_count);

        size_t buffer[2] = {time, result};

        MPI_Send(&buffer, 2, MPI_UNSIGNED_LONG, MASTER_RANK, static_cast<int>(i), MPI_COMM_WORLD);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
        master_routine(world_size);
    } else {
        slave_routine(world_rank, world_size);
    }

    MPI_Finalize();
}
