#include "State.h"
#include "SpriteRenderer.h"
#include "Zombie.h"

State::State() : music("Recursos/audio/BGM.wav"), quitRequested(false) {
    music.Play(-1);

    // Create Background GameObject
    GameObject* bgObj = new GameObject();
    SpriteRenderer* bgRenderer = new SpriteRenderer(*bgObj, "Recursos/img/Background.png");
    bgObj->AddComponent(bgRenderer);
    AddObject(bgObj);

    // Create Zombie GameObject
    GameObject* zombieObj = new GameObject();
    zombieObj->box.x = 600;
    zombieObj->box.y = 450;
    Zombie* zombieComp = new Zombie(*zombieObj);
    zombieObj->AddComponent(zombieComp);
    AddObject(zombieObj);
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
