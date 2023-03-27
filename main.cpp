#include "vkg_shared_ptr.h"
#include <iostream>

template<typename T>
void print_ptr_status(T&& o) {
    std::cout << "Count: " << o.use_count() << " Value: " << *o << std::endl;
}

int main() {
    auto vkg_sp = vkg_shared_ptr<int>(new int(5));
    auto vkg_sp2 = vkg_sp;
    {
        vkg_shared_ptr<int> vkg_sp3;
        vkg_sp3 = vkg_sp2;
        print_ptr_status(vkg_sp3);
    }
    print_ptr_status(std::move(vkg_sp2));
    print_ptr_status(vkg_sp);
    return 0;
}
