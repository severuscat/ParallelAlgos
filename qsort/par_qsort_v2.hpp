#pragma once

#include<vector>
#include<random>
#include <../parlay/parallel.h>
#include <../parlay/primitives.h>
#include <../parlay/sequence.h>

#include "seq_qsort.hpp"

const int BLOCK_SIZE = 1e5;
const int MAX_NUM = 1e3;

namespace par {

    template <typename Range>
    void do_qsort(Range in, Range out) {
    long n = in.size();
    if (n < BLOCK_SIZE) {
        parlay::copy(in, out);
        seq::qsort(out.begin(), out.end());
    } else {

        using T = typename Range::value_type;
        auto pivot = in[n/2];

        parlay::sequence<T> ls = parlay::filter(in, [&](long x){ return x <  pivot; });
        parlay::filter_into_uninitialized(
                in,
                out.cut(ls.size(), n), 
                [&pivot](long x){ return x == pivot; }
            );
        parlay::sequence<T> gt = parlay::filter(in, [&](long x){ return x >  pivot; });

        // std::cout << "\n"  << "\n in\n ";
        // seq::print_vect(in.begin(), in.end());

        // std::cout << "\npivot=" << pivot << "\nls \n ";
        // seq::print_vect(ls.begin(), ls.end());
        // std::cout << "\neq\n";
        // seq::print_vect(eq.begin(), eq.end());
        // std::cout << "\ngt\n";
        // seq::print_vect(gt.begin(), gt.end());


        // parlay::copy(ls, out.cut(0, ls.size()));
        
        // parlay::copy(gt, out.cut(n - gt.size(), n));

        // std::cout << "\nout\n";
        // seq::print_vect(out.begin(), out.end());

        do_qsort(ls.cut(0, ls.size()), out.cut(0,ls.size()));
        do_qsort(gt.cut(0, gt.size()), out.cut(n - gt.size(), n));
    }
}

    template <typename Range>
    auto qsort(Range& in) {
        long n = in.size();
        using T = typename Range::value_type;
        parlay::sequence<T> out(n);
        do_qsort(in.cut(0,n), out.cut(0,n));
        return out;
    }
}
