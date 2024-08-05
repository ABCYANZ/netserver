#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
//#include <cstring>
//#include "Socket.h"
//#include "NetAddress.h"
//#include <vector>
//#include"Eventloop.h"
#include"EchoServer.h"

int main(int argc,char*argv[]) {

    //Epoll ep_; 
    EchoServer loop(argv[1],atoi(argv[2]),3,3);
    loop.Start();
    // Socket fd(socket(AF_INET, SOCK_STREAM, 0));
    // NetAddress addr(argv[1],atoi(argv[2]));
    // fd.setreuseport(1);
    // fd.setkeepalive(1);
    // fd.setnodelay(1);
    // fd.setreuseaddr(1);

    // fd.bind(addr);
    // fd.listen();
    
    // struct epoll_event ev,eve[10];
    // int epfd=epoll_create1(0);
    // ev.events=EPOLLIN;
    // ev.data.fd=fd.fd();
    // if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd.fd(), &ev)<0)
    // {
    //     printf("epoll_ctl() failed(%d).\n",errno); exit(-1);
    // }
    // Epoll ep;
    // channel *ch=new channel(&ep,fd.fd());
    // std::vector<channel*> ev;
    // while (true) 
    // {
    //     ev = move(ep.loop());
    //     for(auto &a:ev)
    //     {
    //         a->handleevent(&fd);
    //     }



        // for(int i=0;i<ev.size();i++)
        // {
        //     if(ev[i].events&EPOLLIN)
        //     {
        //         if(ev[i].data.fd==fd.fd())
        //         {
        //             int clientfd=fd.accept(addr);
        //             ep.addfd(clientfd);
        //             continue;
        //         }
        //         char buffer[1024] = {0};
        //         ssize_t bytes_read = read(ev[i].data.fd, buffer, 1023);
        //         if (bytes_read <= 0) 
        //         {
        //             std::cerr << "Read failed" << std::endl;
        //             close(ev[i].data.fd);
        //             continue;
        //         }

        //         std::cout << "Received: " << std::string(buffer, bytes_read) << std::endl;

        //         if (write(ev[i].data.fd, buffer, bytes_read) != bytes_read) 
        //         {
        //             std::cerr << "Write failed" << std::endl;
        //         }   
        //     }
        
        // }


        // int max=epoll_wait(epfd,eve,10,-1);

        // for(int i=0;i<max;i++)
        // {
        //     if(eve[i].events&EPOLLIN)
        //     {
        //         if(eve[i].data.fd==fd.fd())
        //         {
        //             ev.events=EPOLLIN;
        //             ev.data.fd=fd.accept(addr);
        //             if(epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev)<0)
        //             {
        //                 printf("epoll_ctl() failed(%d).\n",errno); exit(-1);
        //             }
        //             continue;
        //         }
        //         char buffer[1024] = {0};
        //         ssize_t bytes_read = read(eve[i].data.fd, buffer, 1023);
        //         if (bytes_read <= 0) 
        //         {
        //             std::cerr << "Read failed" << std::endl;
        //             close(eve[i].data.fd);
        //             continue;
        //         }

        //         std::cout << "Received: " << std::string(buffer, bytes_read) << std::endl;

        //         if (write(eve[i].data.fd, buffer, bytes_read) != bytes_read) 
        //         {
        //             std::cerr << "Write failed" << std::endl;
        //         }   
        //     }
        
        //}
    return 0;
}
