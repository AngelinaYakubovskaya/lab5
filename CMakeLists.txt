# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(ServerClientApp)

# Стандарт C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Определение переменной для использования библиотеки Winsock на Windows
if(WIN32)
    set(WS2_LIBRARY ws2_32)
endif()

# Добавление исполняемого файла для сервера
add_executable(server main.cpp Server.cpp Server.h)

# Линковка Winsock библиотеки для сервера
if(WIN32)
    target_link_libraries(server ${WS2_LIBRARY})
endif()

# Добавление исполняемого файла для клиента
add_executable(client Client.cpp)

# Линковка Winsock библиотеки для клиента
if(WIN32)
    target_link_libraries(client ${WS2_LIBRARY})
endif()
