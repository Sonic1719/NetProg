#include "echo_server.h"
#include <iostream>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <system_error>

EchoServer::EchoServer(uint16_t port) : port_(port), server_fd_(-1) {}

EchoServer::~EchoServer() {
    if (server_fd_ != -1) {
        close(server_fd_);
    }
}

void EchoServer::create_socket() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ == -1) {
        throw std::system_error(errno, std::generic_category(), "Ошибка создания сокета");
    }

    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void EchoServer::bind_socket() {
    std::unique_ptr<sockaddr_in> address(new sockaddr_in);
    std::memset(address.get(), 0, sizeof(sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY; 
    address->sin_port = htons(port_);

    if (bind(server_fd_, reinterpret_cast<const sockaddr*>(address.get()), sizeof(sockaddr_in)) < 0) {
        throw std::system_error(errno, std::generic_category(), "Ошибка вызова bind()");
    }
}

void EchoServer::listen_socket() {
    if (listen(server_fd_, 3) < 0) {
        throw std::system_error(errno, std::generic_category(), "Ошибка вызова listen()");
    }
}

void EchoServer::handle_client() {
    socklen_t addr_len = sizeof(sockaddr_in);
    std::unique_ptr<sockaddr_in> client_address(new sockaddr_in);
    
    int client_fd = accept(server_fd_, reinterpret_cast<sockaddr*>(client_address.get()), &addr_len);
    if (client_fd < 0) {
        throw std::system_error(errno, std::generic_category(), "Ошибка вызова accept()");
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_address->sin_addr), client_ip, INET_ADDRSTRLEN);
    std::cout << "Клиент подключился! IP: " << client_ip << std::endl;

    char buffer; // Читаем посимвольно (простой буфер)
    while (true) {
        ssize_t received_bytes = recv(client_fd, &buffer, 1, 0);
        
        if (received_bytes < 0) {
            close(client_fd);
            throw std::system_error(errno, std::generic_category(), "Ошибка чтения recv()");
        }
        
        if (received_bytes == 0) {
            std::cout << "Клиент отключился." << std::endl;
            break;
        }

        std::cout << buffer << std::flush; 

        ssize_t sent_bytes = send(client_fd, &buffer, 1, 0);
        if (sent_bytes == -1) {
            close(client_fd);
            throw std::system_error(errno, std::generic_category(), "Ошибка отправки send()");
        }
    }
    close(client_fd);
}

void EchoServer::start() {
    create_socket();
    bind_socket();
    listen_socket();

    std::cout << "TCP Echo-сервер запущен на порту " << port_ << std::endl;
    std::cout << "Ожидание подключения..." << std::endl;
    handle_client(); 
}
