//
// Created by Varun Ganesan on 3/27/23.
//

#include "ctc_codility.h"
#include <iostream>
#include <numeric>
#include <random>

bool is_valid_buffer_sz(const int_vector &A, int Y, int sz) {
    int packets_used = 0, sz_consumed = 0;
    for (const auto &x : A) {
        if (x > sz) {
            // This buffer size is not possible since this message cannot fit
            return false;
        }
        if (sz_consumed + x <= sz) {
            // If x can fit in current packet, include it.
            sz_consumed += x;
        } else {
            packets_used += 1;
            if (packets_used == Y) {
                // We have completely used Y packets AND an extra x in (Y+1)-st packet
                return false;
            }
            sz_consumed = x;
        }
    }
    return true;
}

int solution_packets(int_vector &A, int Y) {
    /*
     * Idea: use binary search to find the correct packet size.
     * Loop invariant: is_valid_buffer_sz(upper) is True while is_valid_buffer_sz(lower) is False.
     * Starting values:
     * - lower = max_element - 1 (since this will never work)
     * - upper = sum of all values (since this is the minimum size for 1 ms time limit)
     */
    if (Y < 1 || A.size() < 1) {
        // If we can't send any packets or there are no messages to send, the task is impossible.
        return -1;
    }
    if (Y >= A.size()) {
        // If we have more ms than messages, the min buffer size required is the max element.
        return *std::max_element(A.begin(), A.end());
    }
    auto lower = *max_element(A.begin(), A.end()) - 1;
    auto upper = std::reduce(A.begin(), A.end());
    while (lower < upper - 1) {
        int mid = (lower + upper) / 2;
        if (is_valid_buffer_sz(A, Y, mid)) {
            upper = mid;
        } else {
            lower = mid;
        }
    }
    return upper;
}

void basic_tests() {
    int_vector vec{ 9 };
    auto y = 2;
    auto sol = solution_packets(vec, y);
    assert(sol == 9);

    vec = { 2, 9, 1};
    sol = solution_packets(vec, y);
    assert(sol == 10);

    vec = { 2, 8, 1, 1, 9, 3 };
    sol = solution_packets(vec, y);
    assert(sol == 12);

    vec = { 1, 1, 1, 2, 2 };
    y = 3;
    sol = solution_packets(vec, y);
    assert(sol == 3);


    vec = { 8, 5, 5 };
    y = 2;
    sol = solution_packets(vec, y);
    assert(sol == 10);

    vec = {};
    y = 3;
    assert(solution_packets(vec, y) == -1);

    auto max_sum = 5 * std::pow(10, 4) * 500;
    std::cout << "MAX POSSIBLE: " << max_sum << " vs MAX ALLOWED: " << std::numeric_limits<int>::max() << std::endl;
    assert(max_sum < std::numeric_limits<int>::max());
}

void generate_test() {
    std::random_device rnd_device;
    std::mt19937 mersenne_engine {rnd_device()};
    std::uniform_int_distribution<int> element_dist {1, 500};
    int n = 50000;
    std::uniform_int_distribution<int> time_limit_dist {1, n-1};

    auto gen = [&element_dist, &mersenne_engine](){
        return element_dist(mersenne_engine);
    };
    std::vector<int> vec(n);
    generate(begin(vec), end(vec), gen);
    auto y = time_limit_dist(mersenne_engine);

    auto sol = solution_packets(vec, y);
    // It should be a valid buffer size and the minimum (1 below should not work)
    assert(is_valid_buffer_sz(vec, y, sol));
    assert(!is_valid_buffer_sz(vec, y, sol-1));
}
