#pragma once
#include"Acceptor.h"
#include"Eventloop.h"
#include<map>
#include <memory>
#include "ThreadPools.h"
//using spConnection=std::shared_ptr<Connection>;
//class Eventloop;
class Tcpserver
{
private:
    Eventloop loop_;
    ThreadPools ThreadPools_;
    std::vector<std::unique_ptr<Eventloop>> loops_;
    std::unique_ptr<Acceptor> sockfd_;
    std::mutex mu_;
    std::map<int,spConnection> conn_;
    
    std::function<void(spConnection,std::string)> readcallback_;
    std::function<void(spConnection)> newConnetioncallback_;
    std::function<void(spConnection)> closecallback_; 
    std::function<void(spConnection)> sendcallback_; 
    std::function<void(spConnection)> errorcallback_;
public:
    Tcpserver(const std::string&ip,uint16_t port,int threadsize);
    ~Tcpserver();

    void start();

    void AddConnetion(std::unique_ptr<Socket> clientfd);

    int CloseConnection(spConnection clientfd);
    void onmessage(spConnection clientfd,std::string buff);
    void WriteCallbac(spConnection clientfd);
    void ErrorConnection(spConnection clientfd);
    

    void setEchoClose(std::function<void(spConnection)> closecallback);
    void setEchoMessage(std::function<void(spConnection,std::string)> readcallback);
    void setEchoError(std::function<void(spConnection)> errorcallback);
    void setEchoNewConnection(std::function<void(spConnection)> newConnetioncallback);
    void setEchoSend(std::function<void(spConnection)> sendcallback);
};