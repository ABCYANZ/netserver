#pragma once
#include"channel.h"
#include"Eventloop.h"
#include <iostream>
#include <memory>
#include"Buffer.h"
#include "TimeStamp.h"
#include<functional>
#include"Socket.h"
class channel;
class Eventloop;
class Connection;
//class Buffer;
using spConnection=std::shared_ptr<Connection>;
class Connection: public std::enable_shared_from_this<Connection>
{
private:
    std::shared_ptr<channel>ch_;
    Eventloop*loop_;
    Buffer rbuff_;
    Buffer wbuff_;
    TimeStamp Stamp_;
    int timeout_;
    std::unique_ptr<Socket>clientfd_;


    std::function<void(spConnection)> closecallback_;
    std::function<void(spConnection,std::string)> onmessagcallback_;
    std::function<void(spConnection)> sendcallback_; 
    std::function<void(spConnection)> errorcallback_;
public:
    Connection(Eventloop*loop,std::unique_ptr<Socket>clientfd);
    ~Connection();

    int fd();

    void onmessage();
    void CloseConnection();
    void WriteCallback(); 
    void ErrorConnection();
    
    void send(const std::string buff);
    void sendLoop(const std::string buff);

    void setclosecallback(std::function<void(spConnection)> closecallback);
    void setonmessagcallback(std::function<void(spConnection,std::string)> onmessagcallback);
    void setsendcallback(std::function<void(spConnection)> sendcallback);
    void seterrorcallback(std::function<void(spConnection)> errorcallback);

    bool Timeout(int out);
};