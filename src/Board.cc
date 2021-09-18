#include "board.hh"

Board::Board()
{
    reset();
}

void Board::reset()
{
    currentPlayer = Tile::Cross;
    nextTilesetIndex = -1;
    metaTiles.reset();

    for (int i = 0; i < TILESET_MAX; ++i) {
        tilesets[i].reset();
    }
}

bool Board::takeTurn(Tile tile, int tilesetIndex, int tileIndex)
{
    if (!validate(tile, tilesetIndex, tileIndex)) return false;

    tilesets[tilesetIndex].data[tileIndex] = tile;

    tilesets[tilesetIndex].analyse(tile);

    // update meta tileset
    if (tilesets[tilesetIndex].state == TilesetState::CrossWon)  metaTiles.data[tilesetIndex] = Tile::Cross;
    if (tilesets[tilesetIndex].state == TilesetState::NaughtWon) metaTiles.data[tilesetIndex] = Tile::Naught;

    metaTiles.analyse(tile);

    // update next tileset index based on current tile index
    if (tilesets[tileIndex].state == TilesetState::Playable) {
        nextTilesetIndex = tileIndex;
    } else {
        nextTilesetIndex = -1;
    }

    // swap current player if game is still playing
    if (metaTiles.state == TilesetState::Playable) {
        if (tile == Tile::Cross) currentPlayer = Tile::Naught;
        if (tile == Tile::Naught) currentPlayer = Tile::Cross;
    }

    return true;
}

bool Board::validate(Tile tile, int tilesetIndex, int tileIndex)
{
    if (metaTiles.state != TilesetState::Playable) return false; // match must still be playable (ie. not won or tied)
    if (tilesets[tilesetIndex].state != TilesetState::Playable) return false; // tileset must still be playable
    if (currentPlayer != tile) return false; // must be player's turn

    // safeguarding array bounds
    if (tilesetIndex < 0 || tilesetIndex >= TILESET_MAX) return false;
    if (tileIndex < 0 || tileIndex >= TILESET_MAX) return false;

    if (tilesets[tilesetIndex].data[tileIndex] != Tile::Empty) return false; // tile must be empty
    if (nextTilesetIndex != -1 && tilesetIndex != nextTilesetIndex) return false; // this is that weird tileset rule

    return true;
}