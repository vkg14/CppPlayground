//
// Created by Varun Ganesan on 3/27/23.
//

#ifndef CPPPLAYGROUND_CTC_CODILITY_H
#define CPPPLAYGROUND_CTC_CODILITY_H

#include <vector>

using int_vector = std::vector<int>;

bool is_valid_buffer_sz(const int_vector &A, int Y, int sz);
int solution_packets(int_vector &A, int Y);
void generate_test();
void basic_tests();

#endif //CPPPLAYGROUND_CTC_CODILITY_H
