#include "seq_qsort.hpp"
#include "par_qsort.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include <cassert>
#include <iomanip>


// const int size = 1e2;
const int size = 1e8;
const int runs = 5;

void gen(std::vector<int>& v) {
    std::random_device rnd_device;
    std::mt19937 mersenne_engine{rnd_device()};
    std::uniform_int_distribution<int> dist{0, MAX_NUM };
    std::generate(v.begin(), v.end(), [&]() -> int { return dist(mersenne_engine); });
}


template <typename It>
void print_vect(It start, It end) {
    for (auto x = start; x != end; ++x) {
        std::cout << *x << " ";
    }
    std::cout << "\n";
}

bool is_sorted(parlay::sequence<int>& v) {
    for (auto i = 1; i < v.size(); i++) {
        if (v[i] < v[i-1]) return false;
    }
    return true;
}

bool is_equal(parlay::sequence<int>& v1, parlay::sequence<int> &v2) {
    for (auto i = 0; i < v1.size(); i++) {
        if (v1[i] != v2[i]) return false;
    }
    return true;
}

int main () {
    std::cout << "START\n";
    double time_seq = 0;
    double time_par = 0;
    std::vector<int> v(size);

    for (uint i = 0; i < runs; ++i) {
        std::cout << "start gen...";
        std::cout.flush();
        gen(v);
        std::cout << "ok!\n";
        auto v_to_seq_sort = parlay::sequence<int>(v.begin(), v.end());
        auto v_to_par_sort = parlay::sequence<int>(v.begin(), v.end());
        // std::sort(v_to_seq_sort.begin(), v_to_seq_sort.end());
        // print_vect(v_to_seq_sort.begin(), v_to_seq_sort.end());

        auto start_par{std::chrono::steady_clock::now()};
        // v_to_par_sort = par::qsort(v_to_par_sort);
        par::do_qsort(v_to_par_sort.begin(), v_to_par_sort.end());
        auto end_par{std::chrono::steady_clock::now()};
        auto duration_par = std::chrono::duration_cast<std::chrono::nanoseconds>(end_par - start_par).count() / 1e9;
        time_par += duration_par;
        std::cout << "iter " << i <<  "\tduration_par" << "\t" << duration_par  << "sec\t";
        std::cout.flush();

        auto start_seq {std::chrono::steady_clock::now()};
        seq::qsort(v_to_seq_sort.begin(), v_to_seq_sort.end());
        // std::sort(v_to_seq_sort.begin(), v_to_seq_sort.end());
        auto end_seq{std::chrono::steady_clock::now()};
        auto duration_seq = std::chrono::duration_cast<std::chrono::nanoseconds>(end_seq - start_seq).count() / 1e9;
        time_seq += duration_seq;
        std::cout <<"duration_seq" << "\t" << duration_seq  << "sec\n";

        //=========CHECK CORRECTNESS========== 

        // print_vect(v_to_seq_sort.begin(), v_to_seq_sort.end());
        // print_vect(v_to_par_sort.begin(), v_to_par_sort.end());
        assert(is_sorted(v_to_seq_sort));
        assert(is_sorted(v_to_par_sort));
        assert(v_to_seq_sort.size() == v_to_par_sort.size());
        assert(is_equal(v_to_seq_sort, v_to_par_sort));
        // print_vect(result.begin(), result.end());

        std::cout <<"check ok!\n";
    }
    
    std::cout << "\n\naverage_time_seq\t" << time_seq / runs << "sec\n";
    std::cout << "average_time_par\t" << time_par / runs << "sec\n";
    std::cout << "boost\t" << (time_seq / runs) / (time_par / runs ) << "\n";
    std::cout << "DONE\n";

    return 0;
}