#include "server.hh"

#include <atomic>
#include <iostream>
#include "Message.hh"
#include "Tileset.hh"

// TODO add a shutdown method that properly removes listener from selector

Server::Server()
{
    running.store(false);
}

void Server::listen(unsigned short port)
{
    auto status = listener.listen(port);
    if (status == sf::Socket::Done) {
        running.store(true);
        // selector ignores invalid sockets/listeners so this has to happen after calling listener.listen()
        selector.add(listener);
        std::cout << "Listening on port " << port << std::endl;
    } else {
        running.store(false);
        std::cout << "Failed to listen on port " << port << std::endl;

    }
}

void Server::shutdown()
{
    running.store(false);
}

void Server::wait()
{
    while (running.load()) {
        if (selector.wait(sf::seconds(10.0f))) {
            if (selector.isReady(listener)) {
                auto client = std::make_shared<sf::TcpSocket>();

                // handle pending connection
                if (listener.accept(*client) == sf::Socket::Done) {
                    clients.push_back(client);
                    selector.add(*client);
                    std::cout << "Accepted new client" << std::endl;
                    std::cout << "no. clients = " << clients.size() << std::endl;
                } else {
                    std::cout << "Failed to accept new client" << std::endl;
                }
            } else {
                auto itr = clients.begin();
                while (itr != clients.end()) {
                    sf::TcpSocket& client = **itr;
                    bool clientConnected = true;
                    
                    // test socket for incoming packet
                    if (selector.isReady(client)) {
                        sf::Packet incoming;
                        auto status = client.receive(incoming);
                        
                        if (status == sf::Socket::Done) {
                            process(client, incoming);
                            std::cout << "Received packet from client" << std::endl;
                        } else if (status == sf::Socket::Disconnected) {
                            clientConnected = false;
                        } else {
                            std::cout << "Failed to receive packet from client" << std::endl;
                        }
                    }

                    // delete socket if disconnected
                    if (clientConnected) {
                        ++itr;
                    } else {
                        selector.remove(client);
                        itr = clients.erase(itr);
                        std::cout << "Client disconnected" << std::endl;
                        std::cout << "no. clients = " << clients.size() << std::endl;
                    }
                }
            }
        }
    }

    // TODO revise this - probably isn't the right way of shutting everything down
    selector.clear();
    clients.clear();
    listener.close();
}

void Server::process(sf::TcpSocket& client, sf::Packet incoming)
{
    Message message;
    incoming >> message;
    
    sf::Packet outgoing;

    if (message == MESSAGE_TURN) {
        int player;
        int tilesetIndex;
        int tileIndex;

        incoming >> player >> tilesetIndex >> tileIndex;
        board.takeTurn(static_cast<Tile>(player), tilesetIndex, tileIndex);

        outgoing << MESSAGE_UPDATE << static_cast<int>(board.currentPlayer) << board.nextTilesetIndex;
        outgoing << static_cast<int>(board.metaTiles.state);

        for (int i = 0; i < TILESET_MAX; ++i) {
            outgoing << static_cast<int>(board.metaTiles.data[i]);
        }

        for (int tsi = 0; tsi < TILESET_MAX; ++tsi) {
            outgoing << static_cast<int>(board.tilesets[tsi].state);
            for (int ti = 0; ti < TILESET_MAX; ++ti) {
                outgoing << static_cast<int>(board.tilesets[tsi].data[ti]);
            }
        }
    }

    //outgoing << MESSAGE_CONNECTION_RESPONSE;
    for (auto itr = clients.begin(); itr != clients.end(); ++itr) {
        send(**itr, outgoing);
    }

}


void Server::send(sf::TcpSocket& client, sf::Packet outgoing)
{
    auto status = client.send(outgoing);
    if (status == sf::Socket::Done) {
        std::cout << "Sent packet to remote" << std::endl;
    } else {
        std::cout << "Failed to send packet to remote" << std::endl;
    }
}