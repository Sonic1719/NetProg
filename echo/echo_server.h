#pragma once

#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>

class EchoServer {
public:
    explicit EchoServer(uint16_t port = 7777);
    ~EchoServer();
    void start();

private:
    uint16_t port_;
    int server_fd_;

    void create_socket();
    void bind_socket();
    void listen_socket();
    void handle_client();
};
