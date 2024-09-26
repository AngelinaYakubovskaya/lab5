#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>

// Необходимо будет добавить библиотеку сокетов при компиляции
#pragma comment(lib, "Ws2_32.lib")

class Server {
public:
    Server(int port);   // Конструктор
    ~Server();          // Деструктор
    void start();       // Запуск сервера

private:
    SOCKET serverSocket, clientSocket;  // Сокеты сервера и клиента
    sockaddr_in serverAddr;             // Структура для адреса сервера
    const int bufferSize = 1024;        // Размер буфера
    char buffer[1024] = { 0 };            // Буфер для передачи данных

    void acceptConnection();            // Приём соединений
    void receiveData();                 // Приём данных
    void sendData();                    // Отправка данных
    void cleanup();                     // Очистка ресурсов
};
#endif
