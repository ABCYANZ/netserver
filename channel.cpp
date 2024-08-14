#include"channel.h"
#include"Socket.h"
//#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "Socket.h"
#include "NetAddress.h"
#include"Eventloop.h"
#include <vector>
channel::channel(Eventloop*loop,int fd):loop_(loop),fd_(fd)
{
    
}
channel::~channel()
{
    //std::cout<<"channel释放 fd="<<fd_<<"\n";
    //std::cout<<"channel id="<<std::this_thread::get_id()<<"\n";
}

int channel::fd()
{
    return fd_;
}
uint32_t channel::events()
{
    return events_;
}
uint32_t channel::revents()
{
    return revents_;
}
bool channel::inepoll()
{
    return inepoll_;
}
void channel::SetEvents(uint32_t events)
{
    events_=events;
}

void channel::SetrEvents(uint32_t revents)
{
    revents_=revents;
}

void channel::SetInepoll(bool inepoll)
{
    inepoll_=inepoll;
}
void channel::Handleevent()
{
    if(revents_&EPOLLHUP)
    {
        closecallback_();
    }
    else if(revents_&EPOLLOUT)
    {
        sendcallback_();
    }
    else if(revents_&(EPOLLIN|EPOLLPRI))
    {
        readcallback_();
    }
    else
    {
        errorcallback_();
    }
}
// void channel::onmessage()
// {
//     char buffer[1024] = {0};
//     ssize_t bytes_read = read(fd_, buffer, 1023);
//     if (bytes_read <= 0) 
//     {
//         std::cerr << "Read failed" << std::endl;
//         close(fd_);
//         return ;
//     }

//     std::cout << "Received: " << std::string(buffer, bytes_read) << std::endl;

//     if (write(fd_, buffer, bytes_read) != bytes_read) 
//     {
//         std::cerr << "Write failed" << std::endl;
//     }   
// }


void channel::StartReading()
{
    //std::cout<<"startreading\n";
    events_|=EPOLLIN;
    loop_->UpdateChannel(this);
}
void channel::Enablewriting()
{
    //std::cout<<"enablewritin fd"<<fd()<<"\n";
    events_|=EPOLLOUT;
    loop_->UpdateChannel(this);
}

void channel::StopReading()
{
    events_&=~EPOLLIN;
    loop_->UpdateChannel(this);
}
void channel::CloseWrite()
{
    events_&=~EPOLLOUT;
    loop_->UpdateChannel(this);
}
void channel::CancelAll()
{   
    events_=0;
    loop_->UpdateChannel(this);
}
void channel::Edge()
{
    events_=EPOLLET;
}

void channel::SetReadConnect(std::function<void()> readcallback)
{
    readcallback_=readcallback;
}
void channel::SetColseConnect(std::function<void()> closecallback)
{
    closecallback_= closecallback;
} 

void channel::SetSendConnect(std::function<void()> sendcallback)
{
    sendcallback_=sendcallback;
}
void channel::SetErrorConnect(std::function<void()> errorcallback)
{
    errorcallback_=errorcallback;
}