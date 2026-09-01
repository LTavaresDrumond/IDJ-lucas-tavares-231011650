#include "Zombie.h"
#include "SpriteRenderer.h"
#include "Animator.h"

Zombie::Zombie(GameObject& associated) : Component(associated), hitpoints(100) {
    SpriteRenderer* sr = new SpriteRenderer(associated, "Recursos/img/Enemy.png", 3, 2);
    associated.AddComponent(sr);

    Animator* anim = new Animator(associated);
    anim->AddAnimation("walking", Animation(0, 3, 10));
    anim->AddAnimation("dead", Animation(5, 5, 0));
    anim->SetAnimation("walking");
    
    associated.AddComponent(anim);
}

void Zombie::Damage(int damage) {
    hitpoints -= damage;
    if (hitpoints <= 0) {
        Animator* anim = associated.GetComponent<Animator>();
        if (anim != nullptr) {
            anim->SetAnimation("dead");
        }
    }
}

void Zombie::Update(float dt) {
    Damage(1);
}

void Zombie::Render() {
}
