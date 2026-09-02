#pragma once

#include "Sprite.h"
#include <string>

class TileSet {
private:
    Sprite tileSet;
    int columns;
    int rows;
    int tileWidth;
    int tileHeight;

public:
    TileSet(int tileWidth, int tileHeight, std::string file);
    void RenderTile(unsigned index, float x, float y);
    int GetTileWidth();
    int GetTileHeight();
};
