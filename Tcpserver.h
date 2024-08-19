#pragma once
#include"Acceptor.h"
#include"Eventloop.h"
#include<map>
#include <memory>
#include "ThreadPools.h"
using spConnection=std::shared_ptr<Connection>;
//class Eventloop;
class Tcpserver
{
private:
    std::unique_ptr<Eventloop> loop_;//主事件循环,负责服务端监听的socket
    ThreadPools ThreadPools_;//线程池,运行从事件循环
    std::vector<std::unique_ptr<Eventloop>> loops_;//从事件循环,负责客户端的socket
    std::unique_ptr<Acceptor> sockfd_;
    std::mutex mu_;//保护conn_
    std::map<int,spConnection> conn_;//已经链接上来的客户端
    
    std::function<void(spConnection,std::string)> readcallback_;//回调上层类的函数,比如echserver,没有就不回调
    std::function<void(spConnection)> newConnetioncallback_;
    std::function<void(spConnection)> closecallback_; 
    std::function<void(spConnection)> sendcallback_; 
    std::function<void(spConnection)> errorcallback_;
public:
    Tcpserver(const std::string&ip,uint16_t port,int threadsize);
    ~Tcpserver();

    void start();
    void stop();
    

    void AddConnetion(Socket*clientfd);//在Acceptor中回调

    void CloseConnection(spConnection clientfd);//在Connection中回调
    void onmessage(spConnection clientfd,std::string buff);//在Connection中回调
    void WriteCallbac(spConnection clientfd);//在Connection中回调
    void ErrorConnection(spConnection clientfd);//在Connection中回调
    

    void setEchoClose(std::function<void(spConnection)> closecallback);
    void setEchoMessage(std::function<void(spConnection,std::string)> readcallback);
    void setEchoError(std::function<void(spConnection)> errorcallback);
    void setEchoNewConnection(std::function<void(spConnection)> newConnetioncallback);
    void setEchoSend(std::function<void(spConnection)> sendcallback);
};