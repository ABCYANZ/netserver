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
    const int fd_;
    std::string ip_;
    uint16_t prot_;
public:
    Socket(int fd);
    ~Socket();

    int fd() const;
    std::string ip();
    uint16_t prot();
    void setipprot(const std::string &ip,uint16_t prot);

    void setreuseaddr(bool no);
    void setnodelay(bool no);
    void setreuseport(bool no);
    void setkeepalive(bool no);

    void bind(const NetAddress &addr);
    void listen(int len=128);
    int accept(NetAddress&addr);
};
