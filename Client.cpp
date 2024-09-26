#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

// Не забудьте добавить библиотеку сокетов при компиляции
#pragma comment(lib, "Ws2_32.lib")

int main() {
    // Инициализация Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // Создание сокета
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // Настройка адреса сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Порт сервера
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // IP-адрес сервера (localhost)

    // Подключение к серверу
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection to server failed" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to the server!" << std::endl;

    // Отправка данных на сервер
    std::string message;
    while (true) {
        std::cout << "Enter message to send (type 'exit' to quit): ";
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message" << std::endl;
            break;
        }

        char buffer[1024] = { 0 };
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            std::cout << "Server response: " << std::string(buffer, 0, bytesReceived) << std::endl;
        }
        else {
            std::cerr << "Failed to receive response" << std::endl;
            break;
        }
    }

    // Закрытие сокета
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}