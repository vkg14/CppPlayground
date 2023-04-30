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
    ThreadPool() = delete;
    explicit ThreadPool(int sz);
    ~ThreadPool();
private:
    int m_sz;
    std::mutex m_q_mutex;
    std::condition_variable m_q_cv;
    std::vector<std::thread> m_pool;
    std::deque<std::function<void()>> m_q;
    std::atomic<bool> m_alive {true};

    void worker_runner();

    void add_job(std::function<void()> fn);
};

#endif //CPPPLAYGROUND_THREAD_POOL_H
