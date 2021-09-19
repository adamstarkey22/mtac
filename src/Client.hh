#ifndef mtac_client_hh
#define mtac_client_hh

#include <SFML/Network.hpp>

class Client {
public:
    Client();
    bool connect(const char* address, unsigned short port);
    bool receive(sf::Packet& incoming);
    void send(sf::Packet& outgoing);

private:
    sf::TcpSocket socket;
    bool connected;
};

#endif