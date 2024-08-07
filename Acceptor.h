#pragma once
#include"Socket.h"
#include"channel.h"
#include"Connection.h"
#include"Eventloop.h"
class Eventloop;
class channel;
class Acceptor
{
private:
    Eventloop *loop_;
    std::unique_ptr<Socket>sockfd_;
    std::shared_ptr<channel>ch_;    
    std::function<void (std::unique_ptr<Socket>)> ConnectionManage_; 

public:
    Acceptor(Eventloop* loop, const std::string& ip, uint16_t port);
    
    ~Acceptor();
    
    void newConnection();
    bool chbool();
    void SetConnectionManage(std::function<void(std::unique_ptr<Socket>)> ConnectionManage);
};