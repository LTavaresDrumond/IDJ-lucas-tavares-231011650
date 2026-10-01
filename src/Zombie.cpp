#include "Zombie.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "InputManager.h"
#include "Camera.h"
#include "Bullet.h"
#include "Character.h"
#include "Collider.h"


int Zombie::aliveCount = 0;

Zombie::Zombie(GameObject& associated) 
    : Component(associated), hitpoints(10), deathSound(associated, "Recursos/audio/Dead.wav"), 
    hitSound(associated, "Recursos/audio/Hit0.wav"), hit(false) {
    
    aliveCount++;
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
        
        Collider* col = associated.GetComponent<Collider>();
        if (col != nullptr) {
            associated.RemoveComponent(col);
            delete col;
        }
    } else {
        hit = true;
        hitTimer.Restart();
        if (anim != nullptr) {
            anim->SetAnimation("hit");
        }
        hitSound.Play(1);
    }
}

Zombie::~Zombie() {
    aliveCount--;
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
    } else if (hitpoints > 0 && Character::player != nullptr) {
        Vec2 playerPos = Character::player->GetAssociated().box.GetCenter();
        Vec2 zombiePos = associated.box.GetCenter();
        
        float distance = zombiePos.Distance(playerPos);
        if (distance > 30.0f) { // Ajuste fino para não sobrepor completamente
            Vec2 dir = (playerPos - zombiePos).GetNormalized();
            float speedMag = 50.0f; // Velocidade do zumbi
            associated.box.x += dir.x * speedMag * dt;
            associated.box.y += dir.y * speedMag * dt;
            
            // Apenas para virar o sprite na horizontal baseado no movimento
            SpriteRenderer* sr = associated.GetComponent<SpriteRenderer>();
            if (sr != nullptr) {
                if (dir.x < 0) {
                    sr->SetFlip(SDL_FLIP_HORIZONTAL);
                } else {
                    sr->SetFlip(SDL_FLIP_NONE);
                }
            }
        }
    }
}

void Zombie::Render() {
}

void Zombie::NotifyCollision(GameObject& other) {
    Bullet* bullet = other.GetComponent<Bullet>();
    if (bullet != nullptr && !bullet->targetsPlayer) {
        Damage(bullet->GetDamage());
    }
}
