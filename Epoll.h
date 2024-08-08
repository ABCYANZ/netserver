#pragma once
#include <sys/epoll.h>
#include<vector>
#include"channel.h"
#include <memory>
class channel;
class Epoll
{
private:
    static const int MaxEvents=100;//epoll,返回事件的数量
    int epollfd;//epoll句柄
    epoll_event events_[MaxEvents]; //接受epoll返回的数组
public:
    Epoll();
    ~Epoll();

    std::vector<std::weak_ptr<channel>> loop();//事件循环

    void UpdateChannel(const std::shared_ptr<channel> &ch);//将channel加入到epoll中

};