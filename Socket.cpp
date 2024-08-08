#include"Socket.h"
#include <unistd.h>
#include <string.h>
#include<signal.h>
// 忽略关闭全部的信号、关闭全部的IO，缺省只忽略信号，不关IO。 
// 不希望后台服务程序被信号打扰，需要什么信号可以在程序中设置。
// 实际上关闭的IO是0、1、2。
void closeioandsignal(bool bcloseio)
{
    int ii=0;

    for (ii=0;ii<64;ii++)
    {
        if (bcloseio==true) close(ii);

        signal(ii,SIG_IGN); 
    }
}
    Socket::Socket(int fd):fd_(fd)
    {

    }
    Socket::~Socket()
    {
       close(fd_);
    }
    int Socket::fd() const
    {
        return fd_;
    }
    std::string Socket::ip()
    {
        return ip_;
    }
    uint16_t Socket::prot()
    {
        return prot_;
    }
    void Socket::setipprot(const std::string &ip,uint16_t prot)
    {
        ip_=ip;
        prot_=prot;
    }
    void Socket::setreuseaddr(bool no)
    {
        int opt=no?1:0;
        setsockopt(fd_,SOL_SOCKET,SO_REUSEADDR,&opt,static_cast<socklen_t>(sizeof opt));    // 必须的。 
    }
    void Socket::setnodelay(bool no)
    {
        int opt=no?1:0;
        setsockopt(fd_,SOL_SOCKET,TCP_NODELAY   ,&opt,static_cast<socklen_t>(sizeof opt));    // 必须的。   
    }
    void Socket::setreuseport(bool no)
    {
        int opt=no?1:0;
        setsockopt(fd_,SOL_SOCKET,SO_REUSEPORT ,&opt,static_cast<socklen_t>(sizeof opt));    // 有用，但是，在Reactor中意义不大。
    }
    void Socket::setkeepalive(bool no)
    {
        int opt=no?1:0;
        setsockopt(fd_,SOL_SOCKET,SO_KEEPALIVE   ,&opt,static_cast<socklen_t>(sizeof opt)); 
    }
    void Socket::bind(const NetAddress &addr)
    {
        struct sockaddr_in serv_addr;
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(addr.ip());
        serv_addr.sin_port = htons(addr.port());
        if(serv_addr.sin_addr.s_addr==INADDR_NONE)
        {
            perror("Error binderror converting the IP");
            exit(-1);
        }
        if(::bind(fd_,(sockaddr *)&serv_addr,sizeof(serv_addr))<0)
        {  
            perror("error with the bind");
            exit(-1);
        }

    }
    void Socket::listen(int len)
    {
        if(::listen(fd_,len)<0)
        {
            perror("error occurred while listen");
            exit(-1);
        }
    }
    int Socket::accept(NetAddress&addr)
    {
        struct sockaddr_in serv_addr;
        socklen_t len = sizeof(serv_addr);
        int clientfd = ::accept4(fd_,(sockaddr*)&serv_addr,&len,SOCK_NONBLOCK);
        addr.setaddr(serv_addr);
        return clientfd;
    }

    int Nonblocking()
    {
        int fd=socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1) 
        {
        
            perror("Socket creation failed");
            return 1;
        }
    
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1) 
        {
            perror("Failed to get socket flags");
            return 1;
        }
    
        flags |= O_NONBLOCK;
        if (fcntl(fd, F_SETFL, flags) == -1) 
        {
            perror("Failed to set socket to non-blocking");
            return 1;
        }
        return fd;
    }
    