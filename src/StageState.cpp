#include "StageState.h"
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
#include "GameData.h"
#include "EndState.h"
#include "Game.h"

StageState::StageState() : tileSet(nullptr), backgroundMusic("Recursos/audio/BGM.wav") {
    // Create Background GameObject
    GameObject* bgObj = new GameObject();
    SpriteRenderer* bgRenderer = new SpriteRenderer(*bgObj, "Recursos/img/Background.png");
    bgRenderer->SetCameraFollower(true);
    bgObj->AddComponent(bgRenderer);
    AddObject(bgObj);

    // Create TileMap GameObject
    GameObject* mapObj = new GameObject();
    tileSet = new TileSet(64, 64, "Recursos/img/Tileset.png");
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
    this->spawner = AddObject(spawnerObj);
}

StageState::~StageState() {
    objectArray.clear();
}

void StageState::LoadAssets() {
}

void StageState::Start() {
    LoadAssets();
    StartArray();
    backgroundMusic.Play(-1);
}

void StageState::Pause() {
    backgroundMusic.Stop();
}

void StageState::Resume() {
    backgroundMusic.Play(-1);
}

void StageState::Update(float dt) {
    InputManager& input = InputManager::GetInstance();
    
    if (input.QuitRequested()) {
        quitRequested = true;
    }

    if (input.KeyPress(ESCAPE_KEY)) {
        popRequested = true;
    }

    Camera::Update(dt);
    UpdateArray(dt);

    if (Character::player == nullptr) {
        GameData::playerVictory = false;
        popRequested = true;
        Game::GetInstance().Push(new EndState());
    } else {
        std::shared_ptr<GameObject> spawnerGo = spawner.lock();
        if (spawnerGo) {
            WaveSpawner* waveSp = spawnerGo->GetComponent<WaveSpawner>();
            if (waveSp && waveSp->IsFinished()) {
                GameData::playerVictory = true;
                popRequested = true;
                Game::GetInstance().Push(new EndState());
            }
        }
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

void StageState::Render() {
    RenderArray();
}
