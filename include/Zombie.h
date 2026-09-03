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
    Zombie(GameObject& associated);
    void Damage(int damage);
    void Update(float dt) override;
    void Render() override;
};
