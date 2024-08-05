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
    std::queue<std::function<void()>> task_;
    std::mutex mu_;
    std::condition_variable condition_;
    std::atomic_bool stop_;
public:
    ThreadPools(int threadsize);
    ~ThreadPools();

    void AddTask(std::function<void()> task);

    size_t size();

    void stop();

};