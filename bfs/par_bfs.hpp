#pragma once

#include<vector>
#include<random>
#include <atomic>
#include "../parlay/parallel.h"
#include "../parlay/primitives.h"
#include "../parlay/sequence.h"

namespace par {
    void par_bfs(const std::vector<std::vector<int> >& graph, parlay::sequence<std::atomic<bool>>& used, 
                int start_vertex_id, parlay::sequence<int>& result) {

        used[start_vertex_id] = true;
        result[start_vertex_id] = 0;
        
        parlay::sequence<int> frontier(1, start_vertex_id);
        parlay::sequence<int> next_frontier;


        while (frontier.size() > 0) {
            parlay::sequence<int> degs(frontier.size());
            parlay::parallel_for(0, frontier.size(), 
                [&](long i){
                    degs[i] = graph[frontier[i]].size();
                }
            );

            auto posn = parlay::scan_inplace(parlay::make_slice(degs), parlay::plus<int>());
            next_frontier.resize(posn);
            parlay::parallel_for(0, frontier.size(),[&](long i) {
                uint v = frontier[i];
                for (uint j = 0; j < graph[v].size(); j++) {
                    uint u =  graph[v][j];
                    bool expected = false;
                    if (used[u].compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
                        next_frontier[degs[i] + j] = u;
                        result[u] = std::min(result[u], result[v] + 1);
                    }
                } 
            });
            frontier =  parlay::filter(next_frontier, [&](long x){ return x > 0; });
            next_frontier.clear();
        }        
    }
}
