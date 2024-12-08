#pragma once

#include<vector>
#include<random>
#include "../parlay/parallel.h"
#include "../parlay/primitives.h"
#include "../parlay/sequence.h"

#include "seq_qsort.hpp"

// const int BLOCK_SIZE = 10;
const int BLOCK_SIZE = 1e6;
const int MAX_NUM = 1e9;

namespace par {
    template <typename Range>
    void do_qsort(Range start, Range end) {

        if (end - start < BLOCK_SIZE) {
            seq::qsort(start, end);
        } else {
            if (start != end && start + 1 != end) {
                auto q = seq::partition(start, end);

                parlay::parallel_do(
                    [&](){do_qsort(start, q);},
                    [&](){do_qsort(q + 1, end);}
                );
            }

        }
    }

    // template <typename Range>
    // void qsort(Range& in, ) {
    //     long n = in.size();
    //     // using T = typename Range::value_type;
    //     // parlay::sequence<T> out(n);
    //     do_qsort(in.begin(), in.end());
    // }
}
