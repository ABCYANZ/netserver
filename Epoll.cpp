#include"Epoll.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

Epoll::Epoll():epollfd(epoll_create1(0))
{

}
Epoll::~Epoll()
{
    close(epollfd);
}

std::vector<std::weak_ptr<channel>>  Epoll::loop()
{
    std::vector<std::weak_ptr<channel>> ev;
    std::shared_ptr<channel> *spch;
    //channel* ch=nullptr; 
    int len=epoll_wait(epollfd,events_,MaxEvents,-1);
    if(len==0)
    {
        return ev;
    }
    else if(len<0)
    {
      perror("epoll_wait() failed"); exit(-1);  
    }
    else if(len>0)
    {
        for(int i=0;i<len;i++)
        {   
            events_[i].data.ptr;
            spch = ((std::shared_ptr<channel>*)events_[i].data.ptr);
            (*spch)->SetrEvents(events_->events);
            ev.emplace_back(*spch);
        }
    }
    return ev;
}

void Epoll::UpdateChannel(const std::shared_ptr<channel>&ch)
{
    struct epoll_event ev;
    ev.events=ch->events();
    ev.data.ptr=&(*ch);
    if(ch->inepoll())
    {
        if(epoll_ctl(epollfd, EPOLL_CTL_MOD, ch->fd(), &ev)<0)
        {
            printf("epoll_ctl() failed(%d).\n",errno); exit(-1);
        }
        return;
    }
    else if(epoll_ctl(epollfd, EPOLL_CTL_ADD, ch->fd(), &ev)<0)
    {
        printf("epoll_ctl() failed(%d).\n",errno); exit(-1);
    }
    ch->SetInepoll(true);
}