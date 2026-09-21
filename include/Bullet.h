#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

class Bullet : public Component {
public:
    Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance);
    
    void Update(float dt) override;
    void Render() override;
    int GetDamage();

private:
    Vec2 speed;
    float distanceLeft;
    int damage;
};
