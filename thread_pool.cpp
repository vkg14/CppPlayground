//
// Created by Varun Ganesan on 4/25/23.
//

#include "thread_pool.h"
#include <iostream>
#include <mutex>

ThreadPool::ThreadPool() : ThreadPool{std::thread::hardware_concurrency()} { }

ThreadPool::ThreadPool(size_t sz) : m_sz(sz)  {
    m_pool.resize(m_sz);
    for (int i = 0; i < m_sz; i++) {
        m_pool.at(i) = std::thread([&]() {worker_runner();});
    }
}

void ThreadPool::worker_runner() {
    while (m_alive) {
        std::unique_lock ul(m_q_mutex);
        m_worker_condition.wait(ul, [&]() { return !m_alive || !m_q.empty(); });
        if (!m_alive) {
            // Stop doing work - lock auto released by unique lock.
            break;
        }
        auto fn = std::move(m_q.front());
        m_q.pop_front();
        // Relinquish lock after popping off queue
        ul.unlock();
        fn();
        if (--m_job_count == 0) {
            // Let waiter know that all tasks are finished.
            m_tasks_finished.notify_one();
        }
    }
    std::cout << "Thread " << std::this_thread::get_id() << " shutting down." << std::endl;
}

void ThreadPool::add_job(std::function<void()> fn) {
    if (m_alive) {
        std::unique_lock ul(m_q_mutex);
        m_q.push_back(std::move(fn));
        m_job_count++;
        m_worker_condition.notify_one();
    }
}

/*
 * wait() - should be called when no more tasks will be enqueued and main-thread wants all tasks processed by worker
 * threads before thread pool is closed.
 */
void ThreadPool::wait() {
    std::unique_lock ul(m_q_mutex);
    m_tasks_finished.wait(ul, [&]() {return m_q.empty();});
}

ThreadPool::~ThreadPool() {
    m_alive = false;
    // You don't need lock to notify all worker threads.
    m_worker_condition.notify_all();
    for (auto& worker_thread : m_pool) {
        worker_thread.join();
    }
}
