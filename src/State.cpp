#include "State.h"
#include "SpriteRenderer.h"
#include "Zombie.h"
#include "TileMap.h"
#include "TileSet.h"
#include "InputManager.h"
#include "Camera.h"
#include "Character.h"
#include "PlayerController.h"
#include "Collider.h"
#include "Collision.h"
#include "WaveSpawner.h"

State::State() : music("Recursos/audio/BGM.wav"), quitRequested(false), started(false) {
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

    // Create Player
    GameObject* playerObj = new GameObject();
    Character* character = new Character(*playerObj, "Recursos/img/Player.png");
    playerObj->AddComponent(character);
    
    PlayerController* controller = new PlayerController(*playerObj);
    playerObj->AddComponent(controller);
    
    Collider* playerCol = new Collider(*playerObj);
    playerObj->AddComponent(playerCol);
    
    Character::player = character;

    playerObj->box.x = 512;
    playerObj->box.y = 512;
    AddObject(playerObj);

    Camera::Follow(playerObj);

    // Wave Spawner
    GameObject* spawnerObj = new GameObject();
    WaveSpawner* spawner = new WaveSpawner(*spawnerObj);
    spawnerObj->AddComponent(spawner);
    AddObject(spawnerObj);
}

State::~State() {
    objectArray.clear();
}

bool State::QuitRequested() {
    return quitRequested;
}

void State::LoadAssets() {
}

void State::Start() {
    LoadAssets();
    for (unsigned i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Start();
    }
    started = true;
}

void State::Update(float dt) {
    InputManager& input = InputManager::GetInstance();
    
    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
    }



    Camera::Update(dt);

    for (unsigned i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Update(dt);
    }

    // Detecção de colisões
    for (unsigned i = 0; i < objectArray.size(); i++) {
        Collider* colA = objectArray[i]->GetComponent<Collider>();
        if (colA == nullptr) continue;

        for (unsigned j = i + 1; j < objectArray.size(); j++) {
            Collider* colB = objectArray[j]->GetComponent<Collider>();
            if (colB == nullptr) continue;

            float angleA = objectArray[i]->angleDeg * 3.14159265f / 180.0f;
            float angleB = objectArray[j]->angleDeg * 3.14159265f / 180.0f;

            if (Collision::IsColliding(colA->box, colB->box, angleA, angleB)) {
                objectArray[i]->NotifyCollision(*objectArray[j]);
                objectArray[j]->NotifyCollision(*objectArray[i]);
            }
        }
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

std::weak_ptr<GameObject> State::AddObject(GameObject* go) {
    std::shared_ptr<GameObject> sharedGo(go);
    objectArray.push_back(sharedGo);
    if (started) {
        sharedGo->Start();
    }
    return std::weak_ptr<GameObject>(sharedGo);
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* go) {
    for (unsigned i = 0; i < objectArray.size(); i++) {
        if (objectArray[i].get() == go) {
            return std::weak_ptr<GameObject>(objectArray[i]);
        }
    }
    return std::weak_ptr<GameObject>();
}
