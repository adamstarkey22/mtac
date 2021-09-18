#include <iostream>
#include <SFML/Graphics.hpp>

#include "Board.hh"
#include "BoardRenderer.hh"

int main(int argc, char** argv)
{
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
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                // NOTE these static casts are just to get the compiler to shut up
                // i dont even know if the function should take floats or ints to begin with
                if (renderer.intersects(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    // board was clicked
                    int tilesetIndex = renderer.getTilesetIndex(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                    int tileIndex = renderer.getTileIndex(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                    board.takeTurn(board.currentPlayer, tilesetIndex, tileIndex);
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