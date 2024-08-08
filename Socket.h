#pragma once
#include<string>
#include"NetAddress.h"
#include<netinet/tcp.h>
#include<strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
void closeioandsignal(bool bcloseio=false);
int Nonblocking();
class Socket
{
private:
    const int fd_;      //socket,在构造函数中初始化
    std::string ip_;    //IP地址
    uint16_t prot_;     //端口
public:
    Socket(int fd);
    ~Socket();

    int fd() const;     //返回socket
    std::string ip();   //返回c++风格字符串表示的IP地址
    uint16_t prot();    //返回端口
    void setipprot(const std::string &ip,uint16_t prot);//设置ip_和prot_


    void setreuseaddr(bool no); //选项允许服务器重启后可以立即绑定到与之前相同的地址和端口
    void setnodelay(bool no);   //禁用 Nagle 算法,减少数据包的延迟
    void setreuseport(bool no); //允许多个套接字绑定到同一个地址和端口。
    void setkeepalive(bool no); //启用心跳机制，定期向对端发送心跳包。

    void bind(const NetAddress &addr);  //绑定端口和地址,创建监听的socket时使用
    void listen(int len=128);           //设置为监听的socket
    int accept(NetAddress&addr);        //从链接队列中取出一个socket
};
