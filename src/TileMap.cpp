#include "TileMap.h"
#include "GameObject.h"
#include "Camera.h"
#include <fstream>
#include <iostream>

TileMap::TileMap(GameObject& associated, std::string file, TileSet* tileSet) 
    : Component(associated), mapWidth(0), mapHeight(0), mapDepth(0) {
    SetTileSet(tileSet);
    Load(file);
}

void TileMap::Load(std::string file) {
    std::ifstream f(file);
    if (!f.is_open()) {
        std::cerr << "Erro ao carregar o arquivo de mapa " << file << std::endl;
        return;
    }

    char c;
    std::string currentNumber = "";
    int parseStage = 0;

    while (f.get(c)) {
        if (c == ',') {
            if (!currentNumber.empty()) {
                int value = std::stoi(currentNumber);
                if (parseStage == 0) { mapWidth = value; parseStage++; }
                else if (parseStage == 1) { mapHeight = value; parseStage++; }
                else if (parseStage == 2) { mapDepth = value; parseStage++; }
                else { tileMatrix.push_back(value - 1); }
                currentNumber = "";
            }
        } else if (c == '-' || (c >= '0' && c <= '9')) {
            currentNumber += c;
        }
    }
}

void TileMap::SetTileSet(TileSet* tileSet) {
    this->tileSet.reset(tileSet);
}

int& TileMap::At(int x, int y, int z) {
    int index = (z * mapWidth * mapHeight) + (y * mapWidth) + x;
    return tileMatrix[index];
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY) {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int tileIndex = At(x, y, layer);
            if (tileIndex >= 0) {
                int tileX = x * tileSet->GetTileWidth() - (cameraX + Camera::pos.x) + associated.box.x;
                int tileY = y * tileSet->GetTileHeight() - (cameraY + Camera::pos.y) + associated.box.y;
                
                tileSet->RenderTile(tileIndex, tileX, tileY);
            }
        }
    }
}

void TileMap::Render() {
    for (int z = 0; z < mapDepth; ++z) {
        RenderLayer(z, 0, 0);
    }
}

int TileMap::GetWidth() {
    return mapWidth;
}

int TileMap::GetHeight() {
    return mapHeight;
}

int TileMap::GetDepth() {
    return mapDepth;
}

void TileMap::Update(float dt) {
}
