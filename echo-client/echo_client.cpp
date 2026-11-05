#include <iostream>
#include <string>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <system_error>

int main() {
    const char* server_ip = "172.16.40.1";
    uint16_t port = 7;

    try {
        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == -1) {
            throw std::system_error(errno, std::generic_category());
        }

        struct timeval timeout {180, 0};
        setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        std::unique_ptr<sockaddr_in> serv_addr(new sockaddr_in);
        std::memset(serv_addr.get(), 0, sizeof(sockaddr_in));
        serv_addr->sin_family = AF_INET;
        serv_addr->sin_port = htons(port);
        serv_addr->sin_addr.s_addr = inet_addr(server_ip);

        int rc = connect(s, reinterpret_cast<const sockaddr*>(serv_addr.get()), sizeof(sockaddr_in));
        if (rc == -1) {
            close(s);
            throw std::system_error(errno, std::generic_category());
        }

        std::string message;
        std::cout << "Введите сообщение для отправки на echo-сервер: ";
        std::getline(std::cin, message);
        message += "\n";

        ssize_t sent_bytes = send(s, message.c_str(), message.length(), 0);
        if (sent_bytes == -1) {
            close(s);
            throw std::system_error(errno, std::generic_category());
        }

        char buffer[1024];
        std::memset(buffer, 0, sizeof(buffer));

        ssize_t received_bytes = recv(s, buffer, sizeof(buffer) - 1, 0);
        if (received_bytes == -1) {
            close(s);
            throw std::system_error(errno, std::generic_category());
        }

        std::cout << "Получено от сервера: " << buffer;

        close(s);

    } catch (const std::system_error& e) {
        std::cerr << "Системная ошибка: " << e.what() << " (код: " << e.code() << ")" << std::endl;
        return 1;
    }

    return 0;
}
