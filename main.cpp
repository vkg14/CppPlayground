#include "vkg_shared_ptr.h"
#include <iostream>

template<typename T>
void print_ptr_status(T&& o) {
    std::cout << "Count: " << o.use_count() << " Value: " << *o << std::endl;
    auto fwd = std::forward<T>(o);
    std::cout << "Count: " << fwd.use_count() << " Value: " << *fwd << std::endl;
}

int main() {
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
    return 0;
}
