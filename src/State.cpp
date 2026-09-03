#include "State.h"
#include "SpriteRenderer.h"
#include "Zombie.h"
#include "TileMap.h"
#include "TileSet.h"
#include "InputManager.h"
#include "Camera.h"

State::State() : music("Recursos/audio/BGM.wav"), quitRequested(false) {
    music.Play(-1);

    // Create Background GameObject
    GameObject* bgObj = new GameObject();
    SpriteRenderer* bgRenderer = new SpriteRenderer(*bgObj, "Recursos/img/Background.png");
    bgRenderer->SetCameraFollower(true);
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
    InputManager& input = InputManager::GetInstance();
    
    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
    }

    if (input.KeyPress(SPACE_KEY)) {
        GameObject* zombieObj = new GameObject();
        zombieObj->box.x = input.GetMouseX() + Camera::pos.x;
        zombieObj->box.y = input.GetMouseY() + Camera::pos.y;
        Zombie* zombieComp = new Zombie(*zombieObj);
        zombieObj->AddComponent(zombieComp);
        AddObject(zombieObj);
    }

    Camera::Update(dt);

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
