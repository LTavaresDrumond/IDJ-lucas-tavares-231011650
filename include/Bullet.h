#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
#include <memory>

class Bullet : public Component {
public:
    Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, bool targetsPlayer, std::weak_ptr<GameObject> shooter);
    
    void Update(float dt) override;
    void Render() override;
    void NotifyCollision(GameObject& other) override;
    int GetDamage();

private:
    Vec2 speed;
    float distanceLeft;
    int damage;

public:
    bool targetsPlayer;
    std::weak_ptr<GameObject> shooter;
};
