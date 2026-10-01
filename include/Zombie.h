#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Sound.h"
#include "Timer.h"

class Zombie : public Component {
private:
    int hitpoints;
    Sound deathSound;
    Sound hitSound;
    Timer hitTimer;
    Timer deathTimer;
    bool hit;

public:
    static int aliveCount;

    GameObject& GetAssociated() { return associated; }

    Zombie(GameObject& associated);
    ~Zombie();
    void Damage(int damage);
    void Update(float dt) override;
    void Render() override;
    void NotifyCollision(GameObject& other) override;
};
