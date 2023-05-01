#include "ctc_codility.h"
#include "thread_pool.h"
#include "vkg_shared_ptr.h"
#include <iostream>
#include <numeric>
#include <random>
#include <unordered_map>
#include <vector>

using int_vector = std::vector<int>;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


template<typename T>
void print_ptr_status(T&& o) {
    std::cout << "Count: " << o.use_count() << " Value: " << *o << std::endl;
    auto fwd = std::forward<T>(o);
    std::cout << "Count: " << fwd.use_count() << " Value: " << *fwd << std::endl;
}

void testing_vkg_sp() {
    auto vkg_sp = vkg_shared_ptr<int>(new int(5));
    // Calls copy constructor
    auto vkg_sp2 = vkg_sp;
    {
        vkg_shared_ptr<int> vkg_sp3;
        // Calls copy assignment.
        vkg_sp3 = vkg_sp2;
        // Internally calls copy constructor and then destructor.
        print_ptr_status(vkg_sp3);
        // Destructor for vkg_sp3 called
    }
    // Internally calls move constructor and then destructor.
    print_ptr_status(std::move(vkg_sp2));
    // Destructors for vkg_sp (final reference) and vkg_sp2 (no-op) called.
}

void ctc_codility_perf() {
    basic_tests();
    auto t1 = high_resolution_clock::now();
    auto n_gen = 20;
    for (int i = 0; i < n_gen; i++) {
        generate_test();
    }
    auto t2 = high_resolution_clock::now();
    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "Took " << ms_int.count() << " ms to run all tests." << std::endl;
    std::cout << "Took " << ms_int.count() / n_gen << " ms roughly to run each generated." << std::endl;
}

void run_thread_pool(size_t threads) {
    // Initialize random before thread pool to ensure destruction AFTER thread pool.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, 300);
    ThreadPool tp{threads};
    for (int i = 0; i < 30; i++) {
        auto fn = [i, &gen, &dis]() {
            int random_num = dis(gen);
            std::this_thread::sleep_for(std::chrono::milliseconds(random_num));
            std::cout << "Job " << i << " completed in thread: " << std::this_thread::get_id() << std::endl;
        };
        tp.add_job(std::move(fn));
    }
    // Wait for all jobs to finish
    tp.wait();
}

void test_thread_pool() {
    size_t n = std::thread::hardware_concurrency();
    std::vector<size_t> pool_sizes{1, n/2, n-1, n, n+3, 2*n};
    std::unordered_map<size_t, int> ms_consumed_per_pool_size;
    for (auto pool_size : pool_sizes) {
        auto t1 = high_resolution_clock::now();
        run_thread_pool(pool_size);
        auto t2 = high_resolution_clock::now();
        auto ms_int = duration_cast<milliseconds>(t2 - t1);
        ms_consumed_per_pool_size.emplace(pool_size, ms_int.count());
    }
    std::cout << "***********RESULTS************" << std::endl;
    std::cout << n << " concurrent hardware threads are supported.\n";
    for (auto & it : ms_consumed_per_pool_size) {
        std::cout << "Took " << it.second << " ms to perform 30 (100 to 300)-ms tasks with " << it.first << " threads." << std::endl;

    }
}

int main() {
    // testing_vkg_sp();
    // ctc_codility_perf();
    test_thread_pool();
}
