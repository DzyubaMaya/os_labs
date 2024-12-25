#cmake_minimum_required(VERSION 3.10)

# Добавляем исполняемый файл 
#add_executable(parent_process parent_process.cpp)
#add_executable(child_process child_process.cpp)


cmake_minimum_required(VERSION 3.14)
project(ZeroMQExample)

# Включение FetchContent
include(FetchContent)

# Загрузка libzmq (основная библиотека ZeroMQ)
FetchContent_Declare(
    libzmq
    GIT_REPOSITORY https://github.com/zeromq/libzmq.git
    GIT_TAG v4.3.5 # Укажите нужную версию
)

# Загрузка cppzmq (C++ обертка для ZeroMQ)
FetchContent_Declare(
    cppzmq
    GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
    GIT_TAG v4.9.0  # Укажите нужную версию
)

# Скачивание и настройка libzmq и cppzmq
FetchContent_MakeAvailable(libzmq cppzmq)

# Создание исполняемого файла
add_executable(server server.cpp)
add_executable(client client.cpp)

target_include_directories(server PRIVATE ${cppzmq_SOURCE_DIR})
target_include_directories(client PRIVATE ${cppzmq_SOURCE_DIR})

# Линковка cppzmq и libzmq
target_link_libraries(server PRIVATE libzmq)
target_link_libraries(client PRIVATE libzmq)