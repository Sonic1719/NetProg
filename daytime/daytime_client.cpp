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
    uint16_t port = 13; 

    try {
        int s = socket(AF_INET, SOCK_DGRAM, 0);
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

        char dummy = 0;
        ssize_t sent_bytes = sendto(s, &dummy, 0, 0, 
                                    reinterpret_cast<const sockaddr*>(serv_addr.get()), 
                                    sizeof(sockaddr_in));
        if (sent_bytes == -1) {
            close(s);
            throw std::system_error(errno, std::generic_category());
        }

        char buffer[256];
        std::memset(buffer, 0, sizeof(buffer));
        
        socklen_t addr_len = sizeof(sockaddr_in);
        std::unique_ptr<sockaddr_in> from_addr(new sockaddr_in);

        ssize_t received_bytes = recvfrom(s, buffer, sizeof(buffer) - 1, 0, 
                                          reinterpret_cast<sockaddr*>(from_addr.get()), 
                                          &addr_len);
        if (received_bytes == -1) {
            close(s);
            throw std::system_error(errno, std::generic_category());
        }

        std::cout << "Ответ от сервера daytime:\n" << buffer << std::endl;

        close(s);

    } catch (const std::system_error& e) {
        std::cerr << "Системная ошибка: " << e.what() << " (код: " << e.code() << ")" << std::endl;
        return 1;
    }

    return 0;
}
