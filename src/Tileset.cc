#include "tileset.hh"

Tileset::Tileset()
{
    reset();
}

void Tileset::reset()
{
    state = TilesetState::Playable;

    for (int i = 0; i < TILESET_MAX; ++i) {
        data[i] = Tile::Empty;
    }
}

void Tileset::analyse(Tile tile)
{
    bool won = false;

    if (data[TILESET_TL] == tile) {
        if (data[TILESET_TC] == tile && data[TILESET_TR] == tile) won = true;
        if (data[TILESET_CL] == tile && data[TILESET_BL] == tile) won = true;
    }

    if (data[TILESET_CC] == tile) {
        if (data[TILESET_TC] == tile && data[TILESET_BC] == tile) won = true;
        if (data[TILESET_CL] == tile && data[TILESET_CR] == tile) won = true;
        if (data[TILESET_TL] == tile && data[TILESET_BR] == tile) won = true;
        if (data[TILESET_BL] == tile && data[TILESET_TR] == tile) won = true;
    }

    if (data[TILESET_BR] == tile) {
        if (data[TILESET_CR] == tile && data[TILESET_TR] == tile) won = true;
        if (data[TILESET_BC] == tile && data[TILESET_BL] == tile) won = true;
    }

    if (won) {
        if (tile == Tile::Cross) {
            state = TilesetState::CrossWon;
            return;
        } else if (tile == Tile::Naught) {
            state = TilesetState::NaughtWon;
            return;
        }
    }

    bool draw = true;

    for (int i = 0; i < TILESET_MAX; ++i) {
        if (data[i] == Tile::Empty) {
            draw = false;
            break;
        }
    }

    if (draw) {
        state = TilesetState::Draw;
        return;
    }
    
    state = TilesetState::Playable;
}