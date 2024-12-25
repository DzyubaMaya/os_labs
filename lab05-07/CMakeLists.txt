#cmake_minimum_required(VERSION 3.10)

# Добавляем исполняемый файл 
#add_executable(parent_process parent_process.cpp)
#add_executable(child_process child_process.cpp)


cmake_minimum_required(VERSION 3.14)
project(ZeroMQExample)

# Включение FetchContent
#include(FetchContent)


# Загрузка libzmq (основная библиотека ZeroMQ)
# FetchContent_Declare(
#     libzmq
#     GIT_REPOSITORY https://github.com/zeromq/libzmq.git
#     GIT_TAG v4.3.5 # Укажите нужную версию
# )

# # Загрузка cppzmq (C++ обертка для ZeroMQ)
# FetchContent_Declare(
#     cppzmq
#     GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
#     GIT_TAG v4.9.0  # Укажите нужную версию
# )

# Скачивание и настройка libzmq и cppzmq
# FetchContent_MakeAvailable(libzmq)
# FetchContent_MakeAvailable(cppzmq)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/opt/homebrew/include)

# Создание исполняемого файла
add_executable(server server.cpp)
add_executable(client client.cpp)

# Линковка cppzmq и libzmq
target_link_libraries(server PRIVATE /opt/homebrew/lib/libzmq.dylib)
target_link_libraries(client PRIVATE /opt/homebrew/lib/libzmq.dylib)