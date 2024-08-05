#include "NetAddress.h"

NetAddress::NetAddress()
{

}

NetAddress::NetAddress(const std::string&ip,uint16_t port)
{
    addr_.sin_port=htons(port);
    addr_.sin_family=AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}
NetAddress::~NetAddress()
{

}
const char* NetAddress::ip() const
{
    return inet_ntoa(addr_.sin_addr);
}
const uint16_t NetAddress::port() const
{
    return ntohs(addr_.sin_port);
}
const sockaddr*NetAddress::addr()const
{
    return (sockaddr*)&addr_;
} 
void NetAddress::setaddr(sockaddr_in addr)
{
    addr_=addr;
}