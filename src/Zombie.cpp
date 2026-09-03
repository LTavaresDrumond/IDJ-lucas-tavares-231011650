#include "Zombie.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "InputManager.h"
#include "Camera.h"

Zombie::Zombie(GameObject& associated) 
    : Component(associated), hitpoints(10), deathSound(associated, "Recursos/audio/Dead.wav"), 
      hitSound(associated, "Recursos/audio/Hit0.wav"), hit(false) {
    
    SpriteRenderer* sr = new SpriteRenderer(associated, "Recursos/img/Enemy.png", 3, 2);
    associated.AddComponent(sr);

    Animator* anim = new Animator(associated);
    anim->AddAnimation("walking", Animation(0, 3, 0.2f));
    anim->AddAnimation("dead", Animation(5, 5, 0));
    anim->AddAnimation("hit", Animation(4, 4, 0)); // Nova animação de hit
    anim->SetAnimation("walking");
    
    associated.AddComponent(anim);
}

void Zombie::Damage(int damage) {
    if (hitpoints <= 0) return; // Se já está morto, não toma dano novamente nem repete o som

    hitpoints -= damage;
    Animator* anim = associated.GetComponent<Animator>();

    if (hitpoints <= 0) {
        if (anim != nullptr) {
            anim->SetAnimation("dead");
        }
        deathSound.Play(1);
    } else {
        hit = true;
        hitTimer.Restart();
        if (anim != nullptr) {
            anim->SetAnimation("hit");
        }
        hitSound.Play(1);
    }
}

void Zombie::Update(float dt) {
    if (hitpoints <= 0) {
        deathTimer.Update(dt);
        if (deathTimer.Get() > 5.0f) {
            associated.RequestDelete();
        }
        return;
    }

    if (hit) {
        hitTimer.Update(dt);
        if (hitTimer.Get() > 0.5f) {
            hit = false;
            Animator* anim = associated.GetComponent<Animator>();
            if (anim != nullptr) {
                anim->SetAnimation("walking");
            }
        }
    }

    InputManager& input = InputManager::GetInstance();
    if (input.MousePress(LEFT_MOUSE_BUTTON)) {
        float mouseX = input.GetMouseX() + Camera::pos.x;
        float mouseY = input.GetMouseY() + Camera::pos.y;
        
        if (associated.box.Contains(mouseX, mouseY)) {
            Damage(2); // Aplica uma quantidade arbitrária de dano (ex: 2 para precisar de 5 hits se hitpoints = 10)
        }
    }
}

void Zombie::Render() {
}
