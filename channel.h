#pragma once
#include"Eventloop.h"
#include"Socket.h"
#include<functional>
#include <memory>
class Eventloop;
class channel:public std::enable_shared_from_this<channel>
{
private:
    Eventloop *loop_;//从构造函数中传入一个channel
    int fd_;//socket一个channel对象对应一个socket,在构造函数中传入
    uint32_t events_;//在epoll上的事件
    uint32_t revents_=0;//已经发生的事件
    bool inepoll_=false;//是否已经在epoll上
    
    std::function<void()> readcallback_;    //读事件的回调函数,如果是有新客户端链接上来就调用Acceptor类中的newConnectio,如果是客户端就调用Connection中的onmessage
    std::function<void()> closecallback_;   //关闭链接的回调函数,回调Connection中的CloseConnection();
    std::function<void()> sendcallback_;    //读事件的回调函数,回调Connection中的WriteCallback();
    std::function<void()> errorcallback_;   //发送错误的回调函数,回调Connection中的ErrorConnection();
public:
    channel(Eventloop*loop,int fd);
    ~channel();

    int fd();//返回fd_
    uint32_t events();//返回在epoll上的事件
    uint32_t revents();//返回已经发生的事件
    bool inepoll();//返回inepoll_成员

    void SetEvents(uint32_t events);//设置events_成员
    void SetrEvents(uint32_t revents);//设置revents_成员
    void SetInepoll(bool include);//设置inepoll_成员

    void Handleevent();//发生事件后的回调函数,在Eventloop.run()函数的中调佣

    void Edge();//设置为边缘触发
    void StartReading();//注册读事件
    void Enablewriting();//注册写事件
    void StopReading();//关闭读事件
    void CloseWrite();//关闭写事件
    void CancelAll();//关闭所有事件

    void SetReadConnect(std::function<void()> readcallback);//设置redcallback_成员
    void SetColseConnect(std::function<void()> closecallback);
    void SetSendConnect(std::function<void()> sendcallback);
    void SetErrorConnect(std::function<void()> errorcallback);
};