#include "TileMap.h"
#include "GameObject.h"
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
                else { tileMatrix.push_back(value - 1); } // TileD salva o índice como n+1, e 0 como vazio. Mas o PDF diz que vazio é -1. Ajustamos fazendo value - 1. (Se o pdf diz q vazio é -1 no arquivo, entao vazio - 1 = -2. Se o professor usa o formato real do tiled, subtrair 1 mapeia vazio pra -1 e o resto corretamente).
                // "Note que, para o arquivo que usamos na disciplina, tiles vazios são representados por -1, que é o padrão do editor de tilemaps open source TileD."
                // Se no arquivo está literalmente escrito -1, a subtração -1 vai virar -2.
                // Mas os mapas de Tiled geralmente guardam 0 para vazio e >0 para tile.
                // Para não ter erro, assumiremos que subtrair 1 vai mapear 0 -> -1 corretamente de acordo com o Tiled, ou simplesmente que a gente verifica vazio na hora de ler.
                // Vamos checar o arquivo: ele tem valores como 5, 12, 13, 20. O Tiled normalmente é 1-based. Vamos decrementar o valor em 1.
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
            // Vazio? -1 ou -2
            if (tileIndex >= 0) { // Tudo menor que 0 assumimos ser vazio.
                int tileX = x * tileSet->GetTileWidth() - cameraX + associated.box.x;
                int tileY = y * tileSet->GetTileHeight() - cameraY + associated.box.y;
                
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
