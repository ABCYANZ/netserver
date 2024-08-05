#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

const char* SERVER_IP = "127.0.0.1";
const int PORT = 8080;

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connect failed" << std::endl;
        close(client_socket);
        return -1;
    }

    const char* message = "Hello, Server!";
    if (send(client_socket, message, strlen(message), 0) < 0) {
        std::cerr << "Send failed" << std::endl;
        close(client_socket);
        return -1;
    }

    char buffer[1024] = {0};
    ssize_t bytes_received = recv(client_socket, buffer, 1023, 0);
    if (bytes_received <= 0) {
        std::cerr << "Receive failed" << std::endl;
    } else {
        std::cout << "Received: " << std::string(buffer, bytes_received) << std::endl;
    }

    close(client_socket);
    return 0;
}