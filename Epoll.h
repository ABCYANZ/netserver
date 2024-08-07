#pragma once
#include <sys/epoll.h>
#include<vector>
#include"channel.h"
#include <memory>
class channel;
class Epoll
{
private:
    static const int MaxEvents=100;
    int epollfd;
    epoll_event events_[MaxEvents];
public:
    Epoll();
    ~Epoll();

    std::vector<std::weak_ptr<channel>> loop();

    void UpdateChannel(const std::shared_ptr<channel> &ch);

};