#include <pvm3.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <iomanip>
#include <vector>
#include <numeric>

#include "algo.h"

void slave_routine(int wr, int ws) {
    size_t perm_count = 500u / ws;
    std::ifstream input("input.txt");

    graph_t graph;
    size_t cnt;
    input >> cnt;

    for (size_t i = 0; i < cnt; ++i) {
        input >> graph;

        auto start = std::chrono::system_clock::now();

        int result = improved::colorize(graph, perm_count);

        auto stop = std::chrono::system_clock::now();
        auto time = static_cast<int>(duration_cast<milliseconds>(stop - start).count());

        int buffer[2] = {time, result};

        pvm_initsend(PvmDataRaw);
        pvm_pkint(buffer, 2, 1);
        pvm_send(pvm_parent(), 1);
    }
}

int get_tasks_count(int argc, char** argv) {
    if (argc < 2) {
        return 4;
    }
    
    int cnt = atoi(argv[1]);

    if (cnt < 0 || cnt > 25) {
        return 4;
    } else {
        return cnt;
    }
}

int main(int argc, char** argv) {
    int wr = pvm_mytid();
    int cnt = get_tasks_count(argc, argv);

    slave_routine(wr, cnt);
    pvm_exit();
}
