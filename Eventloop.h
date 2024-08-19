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
#include<atomic>
class Epoll;
class channel;
class Connection;
using spConnection=std::shared_ptr<Connection>;
class Eventloop {
private:
    Epoll* ep_;  // 使用 Epoll 类的一个实例
    std::mutex connmu_;//保护conn_队列
    std::map<int,spConnection> conn_;//事件循环中的客户端
    int timeout_;//定时器的周期,清理客户端
    int conntimeout_;//客户端的超时时间,单位秒
    std::thread::id threadid_;//事件循环所在的线程的线程id
    std::mutex mu_;//包含task_队列
    std::queue<std::function<void()>> task_;
    int efd_;//eventfd,唤醒事件循环,执行TaskSend函数
    channel*efdch_;//与efd_对应的channel对象
    int timeoutfd_;//定时器的句柄
    channel*timeoutfdch_;//与定时器的句柄对应的实例
    std::function<void(spConnection)> closecallback_;//清理客户端的回调,调用Tcpserver中的CloseConnection(spConnection clientfd);
    std::atomic_bool stop_;//退出事件循环的标志
    bool loop_;
public:
    Eventloop(bool loop,int timeout=30,int conntimeout=60);
    ~Eventloop();

    void run();//事件循环

    void stop();//停止事件循环

    bool CurrentLoop();//判断当前线程是否是事件循环所在线程,保护客户端的发生缓冲区

    void UpdateChannel(channel* ch);  // 确保 channel 类型被正确声明

    void AddConnection(spConnection conn);//
    void TaskSend();//事件循环被efd_唤醒后执行的函数
    void Addtask(std::function<void()> task);//将任务添加到task_队列中
    
    void TimeouEvent();//定时器唤醒后执行的函数,清理超时客户端

    void setclosecallback(std::function<void(spConnection)> closecallback);
};