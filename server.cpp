#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

const int PORT = 8080;

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_socket);
        return -1;
    }

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        char buffer[1024] = {0};
        ssize_t bytes_read = read(client_socket, buffer, 1023);
        if (bytes_read <= 0) {
            std::cerr << "Read failed" << std::endl;
            close(client_socket);
            continue;
        }

        std::cout << "Received: " << std::string(buffer, bytes_read) << std::endl;

        if (write(client_socket, buffer, bytes_read) != bytes_read) {
            std::cerr << "Write failed" << std::endl;
        }

        close(client_socket);
    }

    //close(server_socket);
    return 0;
}