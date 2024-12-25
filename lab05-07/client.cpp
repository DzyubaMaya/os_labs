#include <iostream>
#include <string>
#include <zmq.hpp>

int main() {
    // Контекст ZeroMQ
    zmq::context_t context(1);

    // Сокет типа REQ (запросчик)
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    std::string message;
    std::cout << "Введите сообщение для отправки серверу: ";
    std::getline(std::cin, message);

    // Отправка сообщения серверу
    zmq::message_t request(message.size());
    memcpy(request.data(), message.data(), message.size());
    socket.send(request, zmq::send_flags::none);

    // Получение ответа от сервера
    zmq::message_t reply;
    socket.recv(reply, zmq::recv_flags::none);
    std::string reply_message(static_cast<char*>(reply.data()), reply.size());
    std::cout << "Ответ от сервера: " << reply_message << std::endl;

    return 0;
}