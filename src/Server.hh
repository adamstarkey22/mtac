#ifndef mtac_server_hh
#define mtac_server_hh

#include <memory>
#include <vector>
#include <SFML/Network.hpp>

#include "Board.hh"

class Server {
public:
    Server();
    void listen(unsigned short port);
    void wait();
    void process(sf::TcpSocket& client, sf::Packet incoming);
    void send(sf::TcpSocket& client, sf::Packet outgoing);
    void shutdown();
private:
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::vector<std::shared_ptr<sf::TcpSocket>> clients;
    std::atomic<bool> running;
    Board board;
};

#endif