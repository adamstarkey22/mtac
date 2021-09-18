#ifndef mtac_boardrenderer_hh
#define mtac_boardrenderer_hh

#include <SFML/Graphics.hpp>

#include "Board.hh"

class BoardRenderer {
public:
    BoardRenderer();
    void setScale(float scale);
    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window, Board& board);
    bool intersects(float x, float y);
    int getTilesetIndex(float x, float y);
    int getTileIndex(float x, float y);
private:
    sf::RectangleShape tilesetRect;
    sf::RectangleShape tileRect;
    float tileScale;
    float offsetX;
    float offsetY;
};

#endif