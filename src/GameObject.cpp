#include "GameObject.h"
#include <algorithm>

GameObject::GameObject() : isDead(false) {
}

GameObject::~GameObject() {
    for (int i = components.size() - 1; i >= 0; i--) {
        delete components[i];
    }
    components.clear();
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
}

void GameObject::RemoveComponent(Component* cpt) {
    auto it = std::find(components.begin(), components.end(), cpt);
    if (it != components.end()) {
        components.erase(it);
    }
}
