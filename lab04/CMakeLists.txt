cmake_minimum_required(VERSION 3.10)
# Требования к версии CMake

project(Lib_project)
# Объявляет новый проект с именем Lib_project

set(CMAKE_CXX_STANDARD 20) # Стандарт языка C++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Без соответствия стандарту не будет собран, гарантирует, что будет использован указанный стандарт.

# Добавление путей к заголовочным файлам
include_directories(include)

# Добавление исходных файлов для библиотек
add_library(integral SHARED src/integral.cpp)
add_library(Pi SHARED src/pi.cpp)

# Добавление исполняемых файлов для тестов
add_executable(test_dynamic test/test_dynamic.cpp)
add_executable(test_static test/test_static.cpp)

# Связывание библиотек с исполняемыми файлами
target_link_libraries(test_dynamic PRIVATE integral Pi -ldl)
target_link_libraries(test_static PRIVATE integral Pi)

# Указание пути для поиска библиотек
link_directories(${CMAKE_BINARY_DIR})

# Указание пути для поиска заголовочных файлов
include_directories(${CMAKE_SOURCE_DIR}/include)

# Указание пути для поиска исходных файлов
set(SOURCE_FILES src/integral.cpp src/pi.cpp)

# Указание пути для поиска тестовых файлов
set(TEST_FILES test/test_dynamic.cpp test/test_static.cpp)

# Добавление исполняемых файлов для основных программ
add_executable(dynamic_Lib test/test_dynamic.cpp)
add_executable(Lib_project test/test_static.cpp)

# Связывание библиотек с основными исполняемыми файлами
target_link_libraries(dynamic_Lib PRIVATE integral Pi -ldl)
target_link_libraries(Lib_project PRIVATE integral Pi)