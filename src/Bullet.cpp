#include "Bullet.h"
#include "SpriteRenderer.h"
#include <cmath>

Bullet::Bullet(GameObject& associated, float angle, float speedMag, int damage, float maxDistance)
    : Component(associated), distanceLeft(maxDistance), damage(damage) {
    
    SpriteRenderer* sr = new SpriteRenderer(associated, "Recursos/img/Bullet.png");
    associated.AddComponent(sr);

    speed = Vec2(std::cos(angle), std::sin(angle)) * speedMag;
    
    associated.angleDeg = angle * 180.0f / 3.14159265f;
    
    // Desloca a box para o centro de rotação (ajuste fino opcional, já que a imagem é pequena)
    associated.box.x -= associated.box.w / 2.0f;
    associated.box.y -= associated.box.h / 2.0f;
}

void Bullet::Update(float dt) {
    Vec2 step = speed * dt;
    associated.box.x += step.x;
    associated.box.y += step.y;
    
    float dist = step.Magnitude();
    distanceLeft -= dist;
    
    if (distanceLeft <= 0) {
        associated.RequestDelete();
    }
}

void Bullet::Render() {}

int Bullet::GetDamage() {
    return damage;
}
