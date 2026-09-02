#include "State.h"
#include "SpriteRenderer.h"
#include "Zombie.h"
#include "TileMap.h"
#include "TileSet.h"

State::State() : music("Recursos/audio/BGM.wav"), quitRequested(false) {
    music.Play(-1);

    // Create Background GameObject
    GameObject* bgObj = new GameObject();
    SpriteRenderer* bgRenderer = new SpriteRenderer(*bgObj, "Recursos/img/Background.png");
    bgObj->AddComponent(bgRenderer);
    AddObject(bgObj);

    // Create TileMap GameObject
    GameObject* mapObj = new GameObject();
    TileSet* tileSet = new TileSet(64, 64, "Recursos/img/Tileset.png");
    TileMap* tileMap = new TileMap(*mapObj, "Recursos/map/map.txt", tileSet);
    mapObj->AddComponent(tileMap);
    mapObj->box.x = 0;
    mapObj->box.y = 0;
    AddObject(mapObj);

    // Create Zombie GameObject 1
    GameObject* zombieObj1 = new GameObject();
    zombieObj1->box.x = 600;
    zombieObj1->box.y = 450;
    Zombie* zombieComp1 = new Zombie(*zombieObj1);
    zombieObj1->AddComponent(zombieComp1);
    AddObject(zombieObj1);

    // Create Zombie GameObject 2
    GameObject* zombieObj2 = new GameObject();
    zombieObj2->box.x = 300;
    zombieObj2->box.y = 200;
    Zombie* zombieComp2 = new Zombie(*zombieObj2);
    zombieObj2->AddComponent(zombieComp2);
    AddObject(zombieObj2);

    // Create Zombie GameObject 3
    GameObject* zombieObj3 = new GameObject();
    zombieObj3->box.x = 800;
    zombieObj3->box.y = 300;
    Zombie* zombieComp3 = new Zombie(*zombieObj3);
    zombieObj3->AddComponent(zombieComp3);
    AddObject(zombieObj3);
}

State::~State() {
    objectArray.clear();
}

bool State::QuitRequested() {
    return quitRequested;
}

void State::LoadAssets() {
}

void State::Update(float dt) {
    if (SDL_QuitRequested()) {
        quitRequested = true;
    }

    for (unsigned i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Update(dt);
    }

    for (unsigned i = 0; i < objectArray.size(); i++) {
        if (objectArray[i]->IsDead()) {
            objectArray.erase(objectArray.begin() + i);
            i--; // Adjust index after erase
        }
    }
}

void State::Render() {
    for (unsigned i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Render();
    }
}

void State::AddObject(GameObject* go) {
    objectArray.emplace_back(std::unique_ptr<GameObject>(go));
}
