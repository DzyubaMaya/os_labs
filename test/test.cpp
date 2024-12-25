#include <zmq.hpp>
#include <iostream>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://127.0.0.1:5555");

    std::cout << "ZeroMQ is working in the container!" << std::endl;
    return 0;
}