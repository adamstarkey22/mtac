#include <iostream>
#include <SFML/Graphics.hpp>

#include "Board.hh"

int main(int argc, char** argv)
{
    sf::RenderWindow window(sf::VideoMode(960, 540), "mtac");
    Board board;
    sf::RectangleShape tileRect;
    sf::RectangleShape tilesetRect;

    const int size = 480;
    const int boardX = (960 - size) / 2;
    const int boardY = (540 - size) / 2;

    float tileSize = size / 9.0f;
    
    tileRect.setSize(sf::Vector2f(tileSize, tileSize));
    tileRect.setOutlineThickness(-2.0f);
    tilesetRect.setSize(sf::Vector2f(tileSize * 3, tileSize * 3));
    tilesetRect.setOutlineThickness(-2.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                // test if board was clicked
                bool intersectX = event.mouseButton.x >= boardX && event.mouseButton.x < boardX + size;
                bool intersectY = event.mouseButton.y >= boardY && event.mouseButton.y < boardY + size;
                if (intersectX && intersectY) {
                    // calculate tile index and take turn
                    int tilePositionX = static_cast<int>( (event.mouseButton.x - boardX) / tileSize );
                    int tilePositionY = static_cast<int>( (event.mouseButton.y - boardY) / tileSize );
                    std::cout << "Board was clicked at tile (" << tilePositionX << ", " << tilePositionY << ")" << std::endl;

                    // the / 3 * 3 may seem redundant but its to quantise tilePositionY in thirds
                    // i promise i'm not a complete fuckwit
                    int tilesetIndex = tilePositionY / 3 * 3 + tilePositionX / 3;
                    int tileIndex = tilePositionY % 3 * 3 + tilePositionX % 3;

                    board.takeTurn(board.currentPlayer, tilesetIndex, tileIndex);
                }
            }
        }

        window.clear(sf::Color::Black);

        // render board

        for (int tilesetX = 0; tilesetX < 3; ++tilesetX) {
            for (int tilesetY = 0; tilesetY < 3; ++tilesetY) {
                int tilesetIndex = tilesetY * 3 + tilesetX;
                
                if (board.tilesets[tilesetIndex].state == TilesetState::CrossWon) {
                    tilesetRect.setFillColor(sf::Color::Red);
                } else if (board.tilesets[tilesetIndex].state == TilesetState::NaughtWon) {
                    tilesetRect.setFillColor(sf::Color::Blue);
                } else  {
                    tilesetRect.setFillColor(sf::Color::Transparent);
                }

                if (board.metaTiles.state == TilesetState::Playable && board.tilesets[tilesetIndex].state == TilesetState::Playable && (board.nextTilesetIndex == -1 || board.nextTilesetIndex == tilesetIndex)) {
                    tilesetRect.setOutlineColor(sf::Color::Yellow);
                    tileRect.setOutlineColor(sf::Color::Yellow);
                } else {
                    tilesetRect.setOutlineColor(sf::Color(0x888888FF));
                    tileRect.setOutlineColor(sf::Color(0x888888FF));
                }

                tilesetRect.setPosition(boardX + tilesetX * tileSize * 3, boardY + tilesetY * tileSize * 3);
                window.draw(tilesetRect);

                if (board.tilesets[tilesetIndex].state != TilesetState::Playable) continue;

                for (int tileX = 0; tileX < 3; ++tileX) {
                    for (int tileY = 0; tileY < 3; ++tileY) {
                        int tileIndex = tileY * 3 + tileX;

                        if (board.tilesets[tilesetIndex].data[tileIndex] == Tile::Cross) {
                            tileRect.setFillColor(sf::Color::Red);
                        } else if (board.tilesets[tilesetIndex].data[tileIndex] == Tile::Naught) {
                            tileRect.setFillColor(sf::Color::Blue);
                        } else {
                            tileRect.setFillColor(sf::Color::Transparent);
                        }
                        tileRect.setPosition(boardX + tilesetX * tileSize * 3 + tileX * tileSize, boardY + tilesetY * tileSize * 3 + tileY * tileSize);
                        window.draw(tileRect);
                    }
                }
            }
        }
/*
        for (int x = 0; x < TILESET_MAX; ++x) {
            for (int y = 0; y < TILESET_MAX; ++y) {
                rectangle.setPosition(x * tileSize + boardX, y * tileSize + boardY);

                int tilesetIndex = y / 3 * 3 + x / 3;
                int tileIndex = y % 3 * 3 + x % 3;
                if (board.tilesets[tilesetIndex].data[tileIndex] == Tile::Cross) {
                    rectangle.setFillColor(sf::Color::Red);
                } else if (board.tilesets[tilesetIndex].data[tileIndex] == Tile::Naught) {
                    rectangle.setFillColor(sf::Color::Blue);
                } else {
                    rectangle.setFillColor(sf::Color::Transparent);
                }

                window.draw(rectangle);
            }
        }*/

        window.display();
    }

    return 0;
}