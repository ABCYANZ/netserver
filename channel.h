#pragma once
#include"Eventloop.h"
#include"Socket.h"
#include<functional>
class Eventloop;
class channel
{
private:
    Eventloop *loop_;
    int fd_;
    uint32_t events_;
    uint32_t revents_=0;
    bool inepoll_=false;
    
    std::function<void()> readcallback_;
    std::function<void()> closecallback_; 
    std::function<void()> sendcallback_; 
    std::function<void()> errorcallback_; 
public:
    channel(Eventloop*loop,int fd);
    ~channel();

    int fd();
    uint32_t events();
    uint32_t revents();
    bool inepoll();

    void SetEvents(uint32_t events);
    void SetInepoll(bool include);
    void Handleevent();

    void Edge();
    void StartReading();
    void Enablewriting();
    void StopReading();
    void CloseWrite();
    void CancelAll();

    void SetReadConnect(std::function<void()> readcallback);
    void SetColseConnect(std::function<void()> closecallback);
    void SetSendConnect(std::function<void()> sendcallback);
    void SetErrorConnect(std::function<void()> errorcallback);
};