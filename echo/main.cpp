#include "echo_server.h"
#include <iostream>
#include <system_error>

int main() {
    try {
        EchoServer server(7777);
        server.start();          
    } catch (const std::system_error& e) {
        std::cerr << "Критическая ошибка: " << e.what() << " (код: " << e.code() << ")" << std::endl;
        return 1;
    }
    return 0;
}
