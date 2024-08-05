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
    NetAddress(const std::string&ip,uint16_t port);
    ~NetAddress();
    
    const char* ip() const;//
    const uint16_t port() const;
    const sockaddr*addr()const; 
    void setaddr(sockaddr_in addr);
};