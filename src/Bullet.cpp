#include "Bullet.h"
#include "SpriteRenderer.h"
#include "Collider.h"
#include "Character.h"
#include "Zombie.h"
#include <cmath>

Bullet::Bullet(GameObject& associated, float angle, float speedMag, int damage, float maxDistance, bool targetsPlayer, std::weak_ptr<GameObject> shooter)
    : Component(associated), distanceLeft(maxDistance), damage(damage), targetsPlayer(targetsPlayer), shooter(shooter) {
    
    SpriteRenderer* sr = new SpriteRenderer(associated, "Recursos/img/Bullet.png");
    associated.AddComponent(sr);

    Collider* col = new Collider(associated);
    associated.AddComponent(col);

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

void Bullet::NotifyCollision(GameObject& other) {
    if (other.GetComponent<Bullet>() != nullptr) return; // Ignora outras balas

    // Ignora o próprio atirador
    std::shared_ptr<GameObject> shooterPtr = shooter.lock();
    if (shooterPtr && shooterPtr.get() == &other) return;

    Character* charComp = other.GetComponent<Character>();
    Zombie* zombie = other.GetComponent<Zombie>();

    if (charComp != nullptr) {
        // Bala que targetsPlayer só acerta o player; bala que !targetsPlayer só acerta NPCs
        if (targetsPlayer && charComp == Character::player) {
            associated.RequestDelete();
        } else if (!targetsPlayer && charComp != Character::player) {
            associated.RequestDelete();
        }
    } else if (zombie != nullptr && !targetsPlayer) {
        associated.RequestDelete();
    }
}
