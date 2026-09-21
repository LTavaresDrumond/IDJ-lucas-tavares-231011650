#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Sound.h"
#include "Timer.h"
#include "Vec2.h"
#include <memory>

class Gun : public Component {
public:
    Gun(GameObject& associated, std::weak_ptr<GameObject> character);
    
    void Update(float dt) override;
    void Render() override;
    void Shoot(Vec2 target);

private:
    Sound* shotSound;
    Sound* reloadSound;
    int cooldownState;
    Timer cdTimer;
    std::weak_ptr<GameObject> character;
    float angle;
};
