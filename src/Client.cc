#include "client.hh"

#include <iostream>

Client::Client()
{
    connected = false;
    socket.setBlocking(false);
}

bool Client::connect(const char* address, unsigned short port)
{
    socket.setBlocking(true);

    if (socket.connect(address, port) == sf::Socket::Done) {
        connected = true;
        std::cout << "Connected to remote" << std::endl;
    } else {
        connected = false;
        std::cout << "Failed to connect to remote" << std::endl;
    }

    socket.setBlocking(false);
    return connected;
}

bool Client::receive(sf::Packet& incoming)
{
    if (!connected) return false;

    auto status = socket.receive(incoming);

    if (status == sf::Socket::Done) {
        std::cout << "Received packet from remote" << std::endl;
        return true;
    } else if (status == sf::Socket::Disconnected) {
        std::cout << "Disconnected from remote" << std::endl;
    } else if (status == sf::Socket::Error) {
        std::cout << "Error receiving packet from remote" << std::endl;
    }
    
    return false;

}

void Client::send(sf::Packet& outgoing)
{
    socket.setBlocking(true);

    auto status = socket.send(outgoing);
    if (status == sf::Socket::Done) {
        std::cout << "Sent packet to remote" << std::endl;
    } else {
        std::cout << "Failed to send packet to remote" << std::endl;
    }

    socket.setBlocking(false);
}