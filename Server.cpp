#include "Server.h"
#include <iostream>
#include <string>


// Конструктор сервера
Server::Server(int port) {
    // Инициализация Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Создание сокета
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Привязка сокета к порту
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Прослушивание входящих соединений
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        cleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on port " << port << std::endl;
}

// Деструктор сервера
Server::~Server() {
    cleanup();
}

// Очистка ресурсов
void Server::cleanup() {
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

// Принятие соединений
void Server::acceptConnection() {
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed" << std::endl;
        cleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Client connected" << std::endl;

    // Создание потоков для приёма и отправки данных
    std::thread receiveThread(&Server::receiveData, this);
    std::thread sendThread(&Server::sendData, this);

    // Ожидание завершения потоков
    receiveThread.join();
    sendThread.join();
}

// Приём данных от клиента
void Server::receiveData() {
    while (true) {
        memset(buffer, 0, bufferSize);
        int bytesReceived = recv(clientSocket, buffer, bufferSize, 0);
        if (bytesReceived > 0) {
            std::cout << "Received from client: " << buffer << std::endl;
        }
        else {
            std::cerr << "Receive failed or connection closed by client" << std::endl;
            break;
        }
    }
}

// Отправка данных клиенту
void Server::sendData() {
    while (true) {
        std::string message;
        std::getline(std::cin, message);

        if (send(clientSocket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Send failed" << std::endl;
            break;
        }
    }
}

// Запуск сервера
void Server::start() {
    acceptConnection();
}
