#pragma once

#include<vector>
#include <iostream>
#include<random>

namespace seq {

    template <typename It>
    void print_vect(It start, It end) {
        for (auto x = start; x != end; ++x) {
            std::cout << *x << " ";
        }
        std::cout << "\n";
    }

    template <typename It>
    It partition(It first, It last) {
        auto p = first, i = first, j = last;

        while (true) {
            while(*(++i) < *p) {
                if (i == last - 1) break;
            }
            while(*p < *(--j)) {
                if (j == first) break;
            }
            if (i >= j) break;
            std::swap(*i, *j);
        }
        std::swap(*p, *j);
        return j;
    }

    template <typename It>
    void qsort(It start, It end) {
        // print_vect(start, end);
        if (start != end && start + 1 != end) {
            auto q = partition(start, end);
            qsort(start, q);
            qsort(q + 1, end);
        }
    }
}
