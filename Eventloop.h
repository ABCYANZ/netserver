#pragma once
#include"Epoll.h"
#include"Connection.h"
#include <vector>
#include <memory>
#include <map>
#include <thread>
#include <queue>
#include <mutex>
#include <sys/eventfd.h>
#include<functional>
class Epoll;
class channel;
class Connection;
using spConnection=std::shared_ptr<Connection>;
class Eventloop {
private:
    std::unique_ptr<Epoll> ep_;  // 使用 Epoll 类的一个实例
    std::map<int,spConnection> conn_;
    int timeout_;
    std::thread::id threadid_;
    std::mutex mu_;
    std::queue<std::function<void()>> task_;
    int efd_;
    std::unique_ptr<channel>efdch_;
public:
    Eventloop(int timeout);
    ~Eventloop();

    void run();

    bool CurrentLoop();

    void UpdateChannel(channel* ch);  // 确保 channel 类型被正确声明

    void AddConnection(spConnection conn);
    void TaskSend();
    void Addtask(std::function<void()> task);
};
