#include "BoardRenderer.hh"

BoardRenderer::BoardRenderer()
{
    offsetX = 0;
    offsetY = 0;
    tileRect.setOutlineThickness(-1.0f);
    tilesetRect.setOutlineThickness(-5.0f);
    setScale(50.0f);
}

void BoardRenderer::setScale(float scale)
{
    tileScale = scale / 9.0f;

    // resize drawables
    tileRect.setSize(sf::Vector2f(tileScale, tileScale));
    tilesetRect.setSize(sf::Vector2f(tileScale * 3, tileScale * 3));
}

void BoardRenderer::setPosition(float x, float y)
{
    offsetX = x;
    offsetY = y;
}

void BoardRenderer::draw(sf::RenderWindow& window, Board& board)
{
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
            
            if (board.tilesets[tilesetIndex].state == TilesetState::Playable) {

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

                    tileRect.setPosition(offsetX + tilesetX * tileScale * 3 + tileX * tileScale, offsetY + tilesetY * tileScale * 3 + tileY * tileScale);
                    window.draw(tileRect);
                }
            }

            }

            tilesetRect.setPosition(offsetX + tilesetX * tileScale * 3, offsetY + tilesetY * tileScale * 3);
            window.draw(tilesetRect);
        }
    }
}

bool BoardRenderer::intersects(float x, float y)
{
    bool intersectX = x >= offsetX && x < offsetX + tileScale * 9;
    bool intersectY = y >= offsetY && y < offsetY + tileScale * 9;
    return intersectX + intersectY;
}

int BoardRenderer::getTilesetIndex(float x, float y)
{
    if (!intersects(x, y)) return -1;

    int tilesetX = static_cast<int>( (x - offsetX) / (tileScale * 3) );
    int tilesetY = static_cast<int>( (y - offsetY) / (tileScale * 3) );
    int tilesetIndex = tilesetY * 3 + tilesetX;

    return tilesetIndex;
}

int BoardRenderer::getTileIndex(float x, float y)
{
    if (!intersects(x, y)) return -1;

    int tileX = static_cast<int>( (x - offsetX) / tileScale ) % 3;
    int tileY = static_cast<int>( (y - offsetY) / tileScale ) % 3;
    int tileIndex = tileY * 3 + tileX;

    return tileIndex;
}