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
#include <sys/timerfd.h>
#include <sys/time.h>
class Epoll;
class channel;
class Connection;
using spConnection=std::shared_ptr<Connection>;
class Eventloop {
private:
    std::unique_ptr<Epoll> ep_;  // 使用 Epoll 类的一个实例
    std::mutex connmu_;
    std::map<int,spConnection> conn_;
    int timeout_;
    int conntimeout_;
    std::thread::id threadid_;
    std::mutex mu_;
    std::queue<std::function<void()>> task_;
    int efd_;
    std::shared_ptr<channel>efdch_;
    int timeoutfd_;
    std::shared_ptr<channel>timeoutfdch_;
    std::function<int(spConnection)> closecallback_;
public:
    Eventloop(int timeout=30,int conntimeout=60);
    ~Eventloop();

    void run();

    bool CurrentLoop();

    void UpdateChannel(const std::shared_ptr<channel> &ch);  // 确保 channel 类型被正确声明

    void AddConnection(spConnection conn);
    void TaskSend();
    void Addtask(std::function<void()> task);
    
    void TimeouEvent();
    int CloseConnection(spConnection clientfd);

    void setclosecallback(std::function<int(spConnection)> closecallback);
};