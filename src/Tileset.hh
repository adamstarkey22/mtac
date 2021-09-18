#ifndef mtac_tileset_hh
#define mtac_tileset_hh

enum TileIndex {
    TILESET_TL  = 0,
    TILESET_TC  = 1,
    TILESET_TR  = 2,
    TILESET_CL  = 3,
    TILESET_CC  = 4,
    TILESET_CR  = 5,
    TILESET_BL  = 6,
    TILESET_BC  = 7,
    TILESET_BR  = 8,
    TILESET_MAX = 9
};

#define TILESET_XY(x, y) (y * 3 + x)

enum class Tile {
    Empty, Cross, Naught
};

enum class TilesetState {
    Playable,
    CrossWon,
    NaughtWon,
    Draw
};

class Tileset {
public:
    Tileset();
    void reset();
    void analyse(Tile tile);

    TilesetState state;
    Tile data[TILESET_MAX];
};

#endif