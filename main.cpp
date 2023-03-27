#include "ctc_codility.h"
#include "vkg_shared_ptr.h"
#include <iostream>
#include <numeric>
#include <unordered_set>
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

int solution(int_vector &A) {
    // Implement your solution here
    std::unordered_set<int> m;
    for (const auto &elt : A) {
        if (elt > 0) {
            m.emplace(elt);
        }
    }

    int res = 1;
    while (m.find(res) != m.end()) {
        res += 1;
    }
    return res;
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

int main() {
    auto t1 = high_resolution_clock::now();
    basic_tests();
    auto n_gen = 1000;
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
