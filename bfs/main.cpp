#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include <cassert>
#include <iomanip>
#include "../parlay/parallel.h"
#include "../parlay/primitives.h"
#include "../parlay/sequence.h"
#include "seq_bfs.hpp"
#include "par_bfs.hpp"

const int runs = 5;
const int CUBE_LINEAR_SIZE = 500;


void clear_graph(std::vector<std::vector<int> >& graph) {
    for (uint i = 0; i < graph.size(); i++) {
        graph[i].clear();
    }
    graph.clear();
}

int coord_to_node_id(int i, int j, int k) {
    return i * CUBE_LINEAR_SIZE * CUBE_LINEAR_SIZE + j * CUBE_LINEAR_SIZE + k;
}

void check_same_result(std::vector<int> seq_res, parlay::sequence<int> par_res) {
    assert(seq_res.size() == par_res.size());
    for (int i = 0; i < seq_res.size(); i++) {
        assert(seq_res[i] == par_res[i]);
    }

}


void generate_cubic_graph(std::vector<std::vector<int> >& graph) {
    clear_graph(graph);
    graph.resize(CUBE_LINEAR_SIZE * CUBE_LINEAR_SIZE * CUBE_LINEAR_SIZE);

    for (uint i = 0; i < CUBE_LINEAR_SIZE; i++) {
        for (uint j = 0; j < CUBE_LINEAR_SIZE; j++) {
            for (uint k = 0; k < CUBE_LINEAR_SIZE; k++) {
                int cur_node_id = coord_to_node_id(i, j, k);
                if (i < CUBE_LINEAR_SIZE - 1) {
                    int x = coord_to_node_id(i + 1, j, k);
                    graph[cur_node_id].push_back(x);
                    graph[x].push_back(cur_node_id);
                }
                if (j < CUBE_LINEAR_SIZE - 1) {
                    int y = coord_to_node_id(i, j + 1, k);
                    graph[cur_node_id].push_back(y);
                    graph[y].push_back(cur_node_id);
                }
                if (k < CUBE_LINEAR_SIZE - 1) {
                    int z = coord_to_node_id(i, j, k + 1);
                    graph[cur_node_id].push_back(z);
                    graph[z].push_back(cur_node_id);
                }
            }
        }
    }
}

void print_graph(std::vector<std::vector<int> >& graph) {
    for (int i = 0; i < graph.size(); i++) {
        std::cout << i << "\n";
        for (int j = 0; j < graph[i].size(); j++) {
            std::cout << graph[i][j] << " ";
        }
        std::cout << "\n";
    }
}

int main () {
    std::cout << "START\n";
    std::cout << "start gen graph...";
    std::cout.flush();

    std::vector<std::vector<int> > graph;
    generate_cubic_graph(graph);
    std::cout << "ok!\n";
    std::cout.flush();

    double time_seq = 0;
    double time_par = 0;
    for (uint i = 0; i < runs; i++) {
        // ========================PAR==========================
        parlay::sequence<std::atomic<bool> > used_par(CUBE_LINEAR_SIZE*CUBE_LINEAR_SIZE*CUBE_LINEAR_SIZE);
        parlay::sequence<int> result_par(CUBE_LINEAR_SIZE*CUBE_LINEAR_SIZE*CUBE_LINEAR_SIZE, 0x7fffffff);
        auto start_par{std::chrono::steady_clock::now()};

        par::par_bfs(graph,  used_par,  0, result_par);

        auto end_par{std::chrono::steady_clock::now()};
        auto duration_par = std::chrono::duration_cast<std::chrono::nanoseconds>(end_par - start_par).count() / 1e9;
        time_par += duration_par;
        std::cout << "iter " << i <<  "\tduration_par" << "\t" << duration_par  << "sec\t";
        std::cout.flush();



        // ========================SEQ==========================
        std::vector<bool> used(CUBE_LINEAR_SIZE*CUBE_LINEAR_SIZE*CUBE_LINEAR_SIZE, false);
        std::vector<int> result(CUBE_LINEAR_SIZE*CUBE_LINEAR_SIZE*CUBE_LINEAR_SIZE, 0x7fffffff);
        auto start_seq {std::chrono::steady_clock::now()};

        seq::bfs_seq(graph, used, 0, result);

        auto end_seq{std::chrono::steady_clock::now()};
        auto duration_seq = std::chrono::duration_cast<std::chrono::nanoseconds>(end_seq - start_seq).count() / 1e9;
        time_seq += duration_seq;
        std::cout <<"duration_seq" << "\t" << duration_seq  << "sec\n";
        check_same_result(result, result_par);
    }
    std::cout << "\n\naverage_time_seq\t" << time_seq / runs << "sec\n";
    std::cout << "average_time_par\t" << time_par / runs << "sec\n";
    std::cout << "boost\t" << (time_seq / runs) / (time_par / runs ) << "\n";
    std::cout << "DONE\n";

    return 0;
}  