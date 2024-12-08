#pragma once

#include<vector>
#include<random>
#include <../parlay/parallel.h>
#include <../parlay/primitives.h>
#include <../parlay/sequence.h>

#include "seq_qsort.hpp"


const int BLOCK_SIZE = 1e6;
// const int BLOCK_SIZE = 10;
const int MAX_NUM = 1e2;

namespace par {
    template <typename Range>
    void do_qsort(Range in) {
        long n = in.size();
        if (n < BLOCK_SIZE) {
            // parlay::copy(in, out);
            seq::qsort(in.begin(), in.end());
        } else {
            auto pivot = in[0];

// =======================LESS================================
            auto buf = parlay::sequence<int>(in.size(), 0);
            parlay::parallel_for(
                0,
                n,
                [&](long i){
                    if (in[i] < pivot) buf[i] = 1;
                }
            );

            long ls_num = parlay::scan_inplace(
                parlay::make_slice(buf),
                parlay::plus<int>()
            );

            auto ls = parlay::sequence<int>(ls_num, 0);

            parlay::parallel_for(
                1,
                n,
                [&](long i){
                    if (buf[i] != buf[i-1])
                        ls[buf[i] - 1] = in[i - 1];
                }
            );
            if (ls_num != buf[n - 1]) 
                ls[ls_num - 1] = in[n - 1];


// =======================GT================================
            parlay::parallel_for(
                0,
                n,
                [&](long i) {
                    buf[i] = 0;
                }
            );

            parlay::parallel_for(
                0,
                n,
                [&](long i){
                    if (in[i] > pivot) buf[i] = 1;
                }
            );

            long gt_num = parlay::scan_inplace(
                parlay::make_slice(buf),
                parlay::plus<int>()
            );

            auto gt = parlay::sequence<int>(gt_num, 0);
            
            parlay::parallel_for(
                1,
                n,
                [&](long i){
                    if (buf[i] != buf[i-1])
                        gt[buf[i] - 1] = in[i - 1];
                }
            );
            if (gt_num != buf[n - 1]) 
                gt[gt_num - 1] = in[n - 1];


// =======================PAR DO================================
            parlay::parallel_do(
                [&](){do_qsort(
                    parlay::make_slice(ls)
                );},

                [&](){do_qsort(
                    parlay::make_slice(gt)
                );}
            );
// =======================EQ================================
            parlay::filter_into_uninitialized(
                in,
                in.cut(ls.size(), n), 
                [&pivot](long x){ return x == pivot; }
            );
// =========================================================
            parlay::copy(ls, in.cut(0, ls_num));
            parlay::copy(gt, in.cut(n - gt_num, n));
        }
    }

    template <typename Range>
    auto qsort(Range& in) {
        long n = in.size();
        using T = typename Range::value_type;
        parlay::sequence<T> out(n);
        do_qsort(in.cut(0,n));
        return out;
    }
}
