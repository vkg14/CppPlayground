//
// Created by Varun Ganesan on 4/25/23.
//

#ifndef CPPPLAYGROUND_THREAD_POOL_H
#define CPPPLAYGROUND_THREAD_POOL_H

#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool {
public:
    ThreadPool();
    explicit ThreadPool(size_t sz);
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    void add_job(std::function<void()> fn);
    void wait();
private:
    size_t m_sz;
    std::mutex m_q_mutex;
    std::condition_variable m_worker_condition;
    std::condition_variable m_tasks_finished;
    std::vector<std::thread> m_pool;
    std::deque<std::function<void()>> m_q;
    std::atomic<bool> m_alive {true};

    void worker_runner();

};

#endif //CPPPLAYGROUND_THREAD_POOL_H
