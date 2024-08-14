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
    Eventloop *loop_;//一个Acceptor只对应一个Eventloop,在构造函数中传入
    std::unique_ptr<Socket>sockfd_;//服务端监听的sockfd_
    std::unique_ptr<channel>ch_;    //与Acceptor对应的channel类
    std::function<void (Socket*)> ConnectionManage_; //新链接的回调函数,调用Tcpserver中的AddConnetion

public:
    Acceptor(Eventloop* loop, const std::string& ip, uint16_t port);
    
    ~Acceptor();
    
    void newConnection();//在channel类中回调,处理新链接
    
    void SetConnectionManage(std::function<void(Socket*)> ConnectionManage);//设置回调
};