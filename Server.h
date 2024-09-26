#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>

// ���������� ����� �������� ���������� ������� ��� ����������
#pragma comment(lib, "Ws2_32.lib")

class Server {
public:
    Server(int port);   // �����������
    ~Server();          // ����������
    void start();       // ������ �������

private:
    SOCKET serverSocket, clientSocket;  // ������ ������� � �������
    sockaddr_in serverAddr;             // ��������� ��� ������ �������
    const int bufferSize = 1024;        // ������ ������
    char buffer[1024] = { 0 };            // ����� ��� �������� ������

    void acceptConnection();            // ���� ����������
    void receiveData();                 // ���� ������
    void sendData();                    // �������� ������
    void cleanup();                     // ������� ��������
};
#endif
