//
// Created by Varun Ganesan on 4/18/23.
//

#ifndef CPPPLAYGROUND_VKG_VECTOR_H
#define CPPPLAYGROUND_VKG_VECTOR_H

#include <utility>
#include <memory>
#include <algorithm>
#include <string>
#include <bit>
#include <iostream>

/**
 * Implementation of a custom Vector class providing a subset of the functionality of
 * the standard library vector.
 */
template<typename ElementT, typename AllocatorT = std::allocator<ElementT>>
class Vector
{
  int MIN_CAP { 8 };
  int m_sz { 0 };
  int m_capacity { 0 };
  AllocatorT m_allocator;
  ElementT* m_array { nullptr };

  void resize(int capacity) {
    if (capacity <= m_capacity) {
      return;
    }

    capacity = std::max(MIN_CAP, capacity);
    // Delete the old allocation after copying the contents to new alloc
    auto nxt_array = m_allocator.allocate(capacity);
    for (int i = 0; i < m_sz; i++) {
      nxt_array[i] = m_array[i];
    }
    m_allocator.deallocate(m_array, m_capacity);
    m_capacity = capacity;
    m_array = nxt_array;
  }

  // Array size-doubling
  void double_capacity() {
    resize(std::max(m_capacity*2, MIN_CAP));
  }

public:
    Vector() = default;

    Vector(std::initializer_list<ElementT> elements) {
      resize(elements.size()*2);
      for (auto& t : elements) {
        push_back(t);
      }
    }


    void push_back(const ElementT& element)
    {
      if (m_sz == m_capacity) {
        double_capacity();
      }
      m_array[m_sz] = element;
      m_sz += 1;
    }

    ~Vector() {
      m_allocator.deallocate(m_array, m_capacity);
    }

    [[nodiscard]] int size() const {
      return m_sz;
    }

    ElementT& operator[](int idx) const {
      return m_array[idx];
    }

    bool operator==(const Vector& other) const {
      if (m_sz != other.size()) {
        return false;
      }
      for (int i = 0; i < m_sz; i++) {
        if (m_array[i] != other[i]) {
          return false;
        }
      }
      return true;
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
      if (m_sz == m_capacity) {
        double_capacity();
      }
      // Interestingly m_array[m_sz] = ElementT(std::forward...) does not work.
      new (m_array + m_sz) ElementT(std::forward<Args>(args)...);
      m_sz += 1;
    }

};


#endif //CPPPLAYGROUND_VKG_VECTOR_H
