//
// Created by Varun Ganesan on 4/19/23.
//

#include "vkg_vector.h"
#include <iostream>

int main() {
    Vector<int> v;
    for (int i = 0; i < 10; i++) {
        v.push_back(i);
    }
    assert(v.size() == 10);
    assert(v[0] == 0);
    v[0] = 6;
    assert(v[0] == 6);
    return 0;
}
