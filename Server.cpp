#include "Server.h"
#include <iostream>
#include <string>


// ����������� �������
Server::Server(int port) {
    // ������������� Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // �������� ������
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // ��������� ������ �������
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // �������� ������ � �����
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        cleanup();
        exit(EXIT_FAILURE);
    }

    // ������������� �������� ����������
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        cleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on port " << port << std::endl;
}

// ���������� �������
Server::~Server() {
    cleanup();
}

// ������� ��������
void Server::cleanup() {
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

// �������� ����������
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

    // �������� ������� ��� ����� � �������� ������
    std::thread receiveThread(&Server::receiveData, this);
    std::thread sendThread(&Server::sendData, this);

    // �������� ���������� �������
    receiveThread.join();
    sendThread.join();
}

// ���� ������ �� �������
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

// �������� ������ �������
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

// ������ �������
void Server::start() {
    acceptConnection();
}
