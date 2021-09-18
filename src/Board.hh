#ifndef mtac_board_hh
#define mtac_board_hh

#include "tileset.hh"

class Board {
public:
    Board();
    void reset();
    bool takeTurn(Tile tile, int tilesetIndex, int tileIndex);
    bool validate(Tile tile, int tilesetIndex, int tileIndex);
    
    Tile currentPlayer;
    int nextTilesetIndex;
    Tileset metaTiles;
    Tileset tilesets[TILESET_MAX];
};

#endif