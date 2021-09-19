#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>

#include "Board.hh"
#include "BoardRenderer.hh"
#include "Client.hh"
#include "Server.hh"
#include "Message.hh"

void runServer(Server* server)
{
    server->wait();
}

int main(int argc, char** argv)
{
    const char* address = "localhost";

    if (argc > 1) {
        address = argv[1];
    }

    bool doServer = false;
    if (argc > 2 && strcmp(argv[2], "server") == 0) {
        doServer = true;
    }

    std::thread serverThread;
    Server server;

    if (doServer) {
        server.listen(8080);
        serverThread = std::thread(runServer, &server);
    }
    

    Client client;
    client.connect(address, 8080);

    sf::RenderWindow window(sf::VideoMode(960, 540), "mtac");
    Board board;
    BoardRenderer renderer;

    const int size = 480;
    const int boardX = (960 - size) / 2;
    const int boardY = (540 - size) / 2;

    renderer.setScale(size);
    renderer.setPosition(boardX, boardY);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                if (doServer) {
                    server.shutdown();
                    serverThread.join();
                }
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                // NOTE these static casts are just to get the compiler to shut up
                // i dont even know if the function should take floats or ints to begin with
                if (renderer.intersects(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    // board was clicked
                    int tilesetIndex = renderer.getTilesetIndex(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                    int tileIndex = renderer.getTileIndex(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                    //board.takeTurn(board.currentPlayer, tilesetIndex, tileIndex);

                    sf::Packet outgoing;
                    outgoing << MESSAGE_TURN << static_cast<int>(board.currentPlayer) << tilesetIndex << tileIndex;
                    client.send(outgoing);
                }
            }
        }

        sf::Packet incoming;
        while (client.receive(incoming)) {
            Message message;
            incoming >> message;

            if (message == MESSAGE_UPDATE) {
                int player;
                int nextTilesetIndex;
                int metaTilesState;

                incoming >> player >> nextTilesetIndex >> metaTilesState;
                
                board.currentPlayer = static_cast<Tile>(player);
                board.nextTilesetIndex = nextTilesetIndex;
                board.metaTiles.state = static_cast<TilesetState>(metaTilesState);

                for (int i = 0; i < TILESET_MAX; ++i) {
                    int metaTile;
                    incoming >> metaTile;
                    board.metaTiles.data[i] = static_cast<Tile>(metaTile);
                }

                for (int tsi = 0; tsi < TILESET_MAX; ++tsi) {
                    int tilesetState;
                    incoming >> tilesetState;
                    board.tilesets[tsi].state = static_cast<TilesetState>(tilesetState);
                    
                    for (int ti = 0; ti < TILESET_MAX; ++ti) {
                        int tilesetTile;
                        incoming >> tilesetTile;
                        board.tilesets[tsi].data[ti] = static_cast<Tile>(tilesetTile);
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        // render board
        renderer.draw(window, board);

        window.display();
    }

    return 0;
}