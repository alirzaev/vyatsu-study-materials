#include <pvm3.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <iomanip>
#include <vector>
#include <limits>
#include <chrono>
#include <cstdlib>

using namespace std::chrono;

void master_routine(int ws, int* chs) {
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    size_t cnt;
    input >> cnt;
    output << cnt << std::endl;

    std::vector<int> results(cnt, std::numeric_limits<int>::max()), times(cnt, 0);

    for (size_t i = 0; i < cnt; ++i) {
        int rank_res[2];

        auto g_start = std::chrono::system_clock::now();

        for (int rank = 0; rank < ws; ++rank) {
            pvm_recv(chs[rank], -1);
            pvm_upkint(rank_res, 2, 1);
            
            int time = rank_res[0];
            int res = rank_res[1];

            results[i] = std::min(results[i], res);
            times[i] += time;
        }

        auto g_stop = std::chrono::system_clock::now();
        auto g_time = duration_cast<milliseconds>(g_stop - g_start).count();

        output << std::setw(4) << results[i] << ' '
               << times[i] / ws << std::endl;
        std::cout << g_time << ' ' << times[i] / ws << std::endl;
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
    int cnt = get_tasks_count(argc, argv);
    int* children = new int[cnt];

    std::cout << "Children count: " << cnt << std::endl;

    pvm_mytid();
    pvm_spawn("main_slave", &argv[1], PvmTaskDefault, "", cnt, children);

    master_routine(cnt, children);

    pvm_exit();
    delete[] children;
}
