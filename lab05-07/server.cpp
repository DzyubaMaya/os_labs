#include <iostream>
#include <string>
#include <zmq.hpp>

int main() {
    // Контекст ZeroMQ
    zmq::context_t context(1);

    // Сокет типа REP (ответчик)
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    std::cout << "Сервер запущен и ожидает запросов..." << std::endl;

    while (true) {
        // Получение сообщения от клиента
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);
        std::string message(static_cast<char*>(request.data()), request.size());
        std::cout << "Получено сообщение от клиента: " << message << std::endl;

        // Отправка ответа клиенту
        std::string reply_message = "Ответ от сервера: " + message;
        zmq::message_t reply(reply_message.size());
        memcpy(reply.data(), reply_message.data(), reply_message.size());
        socket.send(reply, zmq::send_flags::none);
    }

    return 0;
}