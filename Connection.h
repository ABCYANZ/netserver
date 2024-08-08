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
    std::shared_ptr<channel>ch_;//一个Connection对应一个channel
    Eventloop*loop_;//监听socket的事件循环,在构造函数中传入
    Buffer rbuff_;//接收缓冲区
    Buffer wbuff_;//发送缓冲区
    TimeStamp Stamp_;//记录客户端的活跃时间
    int timeout_;//客户端的超时时间,单位秒
    std::unique_ptr<Socket>clientfd_;//

    int CloseConnection(spConnection clientfd);
    void onmessage(spConnection clientfd,std::string buff);
    void WriteCallbac(spConnection clientfd);
    void ErrorConnection(spConnection clientfd);
    std::function<void(spConnection)> closecallback_;//关闭客户端的回调,回调Tcpserver中的CloseConnection(spConnection clientfd);
    std::function<void(spConnection,std::string)> onmessagcallback_;//读报文的回调,回调Tcpserver中的onmessage(spConnection clientfd);
    std::function<void(spConnection)> sendcallback_;//回应报文的回调的回调,回调Tcpserver中的WriteCallbac(spConnection clientfd);
    std::function<void(spConnection)> errorcallback_;//发生错误的回调,回调Tcpserver中的ErrorConnection(spConnection clientfd);
public:
    Connection(Eventloop*loop,std::unique_ptr<Socket>clientfd);
    ~Connection();

    int fd();//返回客户端的socket

    void onmessage();//写事件的函数在channel类中回调
    void CloseConnection();//关闭客户端的函数,在channel类中回调
    void WriteCallback(); //发生数据的函数在channel中回调
    void ErrorConnection();//发送错误的函数在channel中回调
    
    void send(const std::string buff);//发送数据的函数
    void sendLoop(const std::string buff);//把回应报文加入发送缓冲区并注册写事件

    void setclosecallback(std::function<void(spConnection)> closecallback);
    void setonmessagcallback(std::function<void(spConnection,std::string)> onmessagcallback);
    void setsendcallback(std::function<void(spConnection)> sendcallback);
    void seterrorcallback(std::function<void(spConnection)> errorcallback);

    bool Timeout(int out);//判断客户端是否超时
};