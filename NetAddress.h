#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
class NetAddress
{
private:
    sockaddr_in addr_; 
public:
    NetAddress();
    NetAddress(const std::string&ip,uint16_t port);//设置addr_成员的ip和端口
    ~NetAddress();
    
    const char* ip() const;//返回字符串表示的ip
    const uint16_t port() const;//返回端口
    const sockaddr*addr()const; //返回addr_成员变量,转换为sockaddr类型
    void setaddr(sockaddr_in addr);//设置addr_变量
};