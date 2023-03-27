//
// Created by Varun Ganesan on 3/26/23.
//

#ifndef CPPPLAYGROUND_VKG_SHARED_PTR_H
#define CPPPLAYGROUND_VKG_SHARED_PTR_H

#include <cstdint>
#include <iostream>
#include <utility>

template<typename T>
class vkg_shared_ptr
{
private:
    T* m_ptr{ nullptr };
    uint64_t* m_counter{ nullptr };

    void init_helper(T* o_ptr, uint64_t* o_counter = nullptr) {
        m_ptr = o_ptr;
        m_counter = o_counter;
        if (m_ptr && m_counter) {
            *m_counter += 1;
        } else if (m_ptr) {
            m_counter = new uint64_t(1);
        }
    }

    void delete_helper() {
        if (m_ptr) {
            std::cout << "Decrementing value from " << *m_counter << " to " << *m_counter - 1 << std::endl;
            *m_counter -= 1;
            if (*m_counter == 0) {
                delete m_ptr;
                delete m_counter;
            }
        } else {
            std::cout << "Deleting an empty object - noop." << std::endl;
        }
    }

public:

    // Default constructor, which delegates to the T* constructor
    vkg_shared_ptr() : vkg_shared_ptr{ nullptr } {
    }

    explicit vkg_shared_ptr(T* ptr) {
        init_helper(ptr);
    }

    // Copy constructor
    vkg_shared_ptr(const vkg_shared_ptr& vkgSharedPtr) noexcept {
        std::cout << "Called copy constructor." << std::endl;
        init_helper(vkgSharedPtr.m_ptr, vkgSharedPtr.m_counter);
    }

    // Move constructor with simple initializer list.
    // std::exchange replaces the reference passed with second argument and returns old value.
    vkg_shared_ptr(vkg_shared_ptr&& o)
            : m_ptr{ std::exchange(o.m_ptr, nullptr) },
              m_counter{ std::exchange(o.m_counter, nullptr) } {

        std::cout << "Called move constructor." << std::endl;
    }

    vkg_shared_ptr(std::unique_ptr<T>&& o) : vkg_shared_ptr{ o.release() }{
        // Release the contents of the unique pointer and init shared ptr.
    }

    // Copy assignment
    vkg_shared_ptr& operator=(const vkg_shared_ptr& vkgSharedPtr) {
        std::cout << "Called copy assignment." << std::endl;
        delete_helper();
        init_helper(vkgSharedPtr.m_ptr, vkgSharedPtr.m_counter);
        return *this;
    }

    // Move assignment
    vkg_shared_ptr& operator=(vkg_shared_ptr&& o) {
        std::cout << "Called move assignment." << std::endl;
        delete_helper();
        m_ptr = std::exchange(o.m_ptr, nullptr);
        m_counter = std::exchange(o.m_counter, nullptr);
        return *this;
    }

    T& operator*() const noexcept {
        return *m_ptr;
    }

    T* operator->() const noexcept {
        // -> returns a pointer??
        return m_ptr;
    }

    void reset(T* ptr) {
        delete_helper();
        *this = vkg_shared_ptr(ptr);
    }

    T* get() const {
        return m_ptr;
    }

    uint64_t use_count() const {
        return *m_counter;
    }

    explicit operator bool() const noexcept {
        return m_ptr != nullptr;
    }

    ~vkg_shared_ptr() {
        std::cout << "Destructor called." << std::endl;
        delete_helper();
    }

};

#endif //CPPPLAYGROUND_VKG_SHARED_PTR_H
