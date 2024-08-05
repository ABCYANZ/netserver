#pragma once
#include <sys/epoll.h>
#include<vector>
#include"channel.h"

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

    std::vector<channel *>loop();

    void UpdateChannel(channel *ch);

};