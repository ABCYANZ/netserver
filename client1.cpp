#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include<strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
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
const char* SERVER_IP = "127.0.0.1";
const int PORT = 8080;

int main() {
    int client_socket = Nonblocking();
    if (client_socket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);
    connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    // if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    //     std::cerr << "Connect failed" << std::endl;
    //     close(client_socket);
    //     return -1;
    // }
    sleep(1);
    int z=0;
    for(int i=0,len=0;i<10000;i++)
    {
        const char* message = "Hello, Server!";
        len=strlen(message);
        if (send(client_socket, &len, sizeof(len), 0) < 0) {
            std::cerr << "Send failed" << std::endl;
            close(client_socket);
            return -1;
        }
        if (send(client_socket, message, strlen(message), 0) < 0) {
            std::cerr << "Send failed" << std::endl;
            close(client_socket);
            return -1;
        }
        z++;int n=0;
        char buffer[1024] = {0};std::string str;
        while (z>0)
        {
            
            ssize_t bytes_received1 = recv(client_socket, &len, 4, 0);
            if(bytes_received1==-1)continue;
            while(1)
            {
                ssize_t bytes_received = recv(client_socket, buffer, len, 0);
                if (bytes_received <= 0) {
                    std::cerr << "Receive failed" << std::endl;
                } 
                else if(bytes_received>0)
                {
                    n+=bytes_received;
                    str.append(buffer,bytes_received);
                }
                if(n==len) {
                    std::cout << "Received: " << str<< std::endl;z--;n=0;break;
                }
            }
        } 
    }
    int len=0;char buffer[1024] = {0};int n=0;std::string str;
        while (z>0)
        {
            
            ssize_t bytes_received1 = recv(client_socket, &len, 4, 0);
            if(bytes_received1==-1)continue;
            while(1)
            {
                ssize_t bytes_received = recv(client_socket, buffer, len, 0);
                if (bytes_received <= 0) {
                    std::cerr << "Receive failed" << std::endl;
                } 
                else if(bytes_received>0)
                {
                    n+=bytes_received;
                    str.append(buffer,bytes_received);
                }
                if(n==len) {
                    std::cout << "Received: " << str<< std::endl;z--;n=0;break;
                }
            }
        } 
   sleep(2); 
    close(client_socket);
    return 0;
}