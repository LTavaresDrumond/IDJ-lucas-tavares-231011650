#include "GameObject.h"
#include <algorithm>

GameObject::GameObject() : isDead(false), started(false), angleDeg(0) {
}

GameObject::~GameObject() {
    components.clear();
}

void GameObject::Start() {
    for (unsigned i = 0; i < components.size(); i++) {
        components[i]->Start();
    }
    started = true;
}

void GameObject::Update(float dt) {
    for (unsigned i = 0; i < components.size(); i++) {
        components[i]->Update(dt);
    }
}

void GameObject::Render() {
    for (unsigned i = 0; i < components.size(); i++) {
        components[i]->Render();
    }
}

bool GameObject::IsDead() const {
    return isDead;
}

void GameObject::RequestDelete() {
    isDead = true;
}

void GameObject::AddComponent(Component* cpt) {
    components.emplace_back(cpt);
    if (started) {
        cpt->Start();
    }
}

void GameObject::RemoveComponent(Component* cpt) {
    auto it = std::find_if(components.begin(), components.end(),
        [cpt](const std::unique_ptr<Component>& ptr) { return ptr.get() == cpt; });
    if (it != components.end()) {
        components.erase(it);
    }
}

void GameObject::NotifyCollision(GameObject& other) {
    for (unsigned i = 0; i < components.size(); i++) {
        components[i]->NotifyCollision(other);
    }
}
