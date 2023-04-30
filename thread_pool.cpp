//
// Created by Varun Ganesan on 4/25/23.
//

#include "thread_pool.h"
#include <iostream>
#include <mutex>

ThreadPool::ThreadPool(int sz) : m_sz(sz)  {
    m_pool.resize(m_sz);
    for (int i = 0; i < m_sz; i++) {
        m_pool.at(i) = std::thread([&]() {worker_runner();});
    }
}

void ThreadPool::worker_runner() {
    while (m_alive) {
        std::unique_lock ul(m_q_mutex);
        while (m_alive && m_q.empty()) {
            m_q_cv.wait(ul);
        }
        if (!m_alive) {
            // Stop doing work - lock auto released by unique lock.
            return;
        }
        auto fn = m_q.front();
        m_q.pop_front();
        // Relinquish lock after popping off queue
        ul.unlock();
        std::cout << "Thread " << std::this_thread::get_id() << " doing work!" << std::endl;
        fn();
    }
}

void ThreadPool::add_job(std::function<void()> fn) {
    if (m_alive) {
        std::unique_lock ul(m_q_mutex);
        m_q.push_back(fn);
    }
    m_q_cv.notify_one();
}

ThreadPool::~ThreadPool() {
    m_alive = false;
    // You don't need lock to notify all
    m_q_cv.notify_all();
    for (auto& active_thread : m_pool) {
        active_thread.join();
    }
}
