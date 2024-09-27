#include "Server.h"

int main() {
    // Создание и запуск сервера на порту 8080
    Server server(8080);
    std::cout << "Waiting for client connections..." << std::endl;
    server.start();
    return 0;
}
