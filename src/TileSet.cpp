#include "TileSet.h"

TileSet::TileSet(int tileWidth, int tileHeight, std::string file) 
    : tileSet(file), tileWidth(tileWidth), tileHeight(tileHeight) {
    if (tileSet.IsOpen()) {
        columns = tileSet.GetWidth() / tileWidth;
        rows = tileSet.GetHeight() / tileHeight;
    } else {
        columns = 0;
        rows = 0;
    }
}

void TileSet::RenderTile(unsigned index, float x, float y) {
    if (index >= 0 && index < (unsigned)(columns * rows)) {
        int tileX = (index % columns) * tileWidth;
        int tileY = (index / columns) * tileHeight;

        tileSet.SetClip(tileX, tileY, tileWidth, tileHeight);
        tileSet.Render(x, y, tileWidth, tileHeight);
    }
}

int TileSet::GetTileWidth() {
    return tileWidth;
}

int TileSet::GetTileHeight() {
    return tileHeight;
}
