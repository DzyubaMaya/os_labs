project(test)

# Найти библиотеку ZeroMQ
#find_package(ZeroMQ REQUIRED)

# Добавить исполняемый файл
add_executable(test test.cpp)

# Связать библиотеку ZeroMQ с вашим проектом
#target_link_libraries(test PRIVATE ZeroMQ::ZeroMQ)