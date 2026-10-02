#include "State.h"

State::State() : popRequested(false), quitRequested(false), started(false) {
}

State::~State() {
    objectArray.clear();
}

bool State::PopRequested() {
    return popRequested;
}

bool State::QuitRequested() {
    return quitRequested;
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

void State::StartArray() {
    for (unsigned i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Start();
    }
    started = true;
}

void State::UpdateArray(float dt) {
    for (unsigned i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Update(dt);
    }
    for (unsigned i = 0; i < objectArray.size(); i++) {
        if (objectArray[i]->IsDead()) {
            objectArray.erase(objectArray.begin() + i);
            i--;
        }
    }
}

void State::RenderArray() {
    for (unsigned i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Render();
    }
}
