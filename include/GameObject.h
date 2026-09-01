#pragma once

#include <vector>
#include "Rect.h"
#include "Component.h"

class GameObject {
private:
    std::vector<Component*> components;
    bool isDead;

public:
    Rect box;

    GameObject();
    ~GameObject();

    void Update(float dt);
    void Render();
    bool IsDead() const;
    void RequestDelete();
    void AddComponent(Component* cpt);
    void RemoveComponent(Component* cpt);

    template <typename T>
    T* GetComponent() {
        for (unsigned i = 0; i < components.size(); i++) {
            T* cpt = dynamic_cast<T*>(components[i]);
            if (cpt != nullptr) {
                return cpt;
            }
        }
        return nullptr;
    }
};
