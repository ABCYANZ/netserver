#pragma once
#include <thread>
#include <memory>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPools
{
private:
    std::vector<std::thread> polls_;
    std::queue<std::function<void()>> task_;//任务队列
    std::mutex mu_;//保护任务队列
    std::condition_variable condition_;//条件变量
    std::atomic_bool stop_;
public:
    ThreadPools(int threadsize);
    ~ThreadPools();

    void AddTask(std::function<void()> task);

    size_t size();

    void stop();

};