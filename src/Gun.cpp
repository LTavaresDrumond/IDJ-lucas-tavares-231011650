#include "Gun.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Game.h"
#include "State.h"
#include "Bullet.h"
#include "InputManager.h"
#include "Camera.h"
#include "Character.h"
#include <cmath>

Gun::Gun(GameObject& associated, std::weak_ptr<GameObject> character) 
    : Component(associated), cooldownState(0), character(character), angle(0) {
    
    SpriteRenderer* sr = new SpriteRenderer(associated, "Recursos/img/Gun.png", 3, 2);
    associated.AddComponent(sr);

    Animator* anim = new Animator(associated);
    anim->AddAnimation("idle", Animation(0, 0, 0.0f));
    anim->AddAnimation("reloading", Animation(0, 5, 0.1f));
    associated.AddComponent(anim);
    anim->SetAnimation("idle");

    shotSound = new Sound(associated, "Recursos/audio/Range.wav");
    reloadSound = new Sound(associated, "Recursos/audio/PumpAction.mp3");
    
    associated.AddComponent(shotSound);
    associated.AddComponent(reloadSound);
}

void Gun::Update(float dt) {
    std::shared_ptr<GameObject> charPtr = character.lock();
    if (!charPtr) {
        associated.RequestDelete();
        return;
    }

    associated.box.x = charPtr->box.x + charPtr->box.w / 2.0f - associated.box.w / 2.0f;
    associated.box.y = charPtr->box.y + charPtr->box.h / 2.0f - associated.box.h / 2.0f;

    Character* charCmp = charPtr->GetComponent<Character>();
    Vec2 center(associated.box.x + associated.box.w / 2.0f, associated.box.y + associated.box.h / 2.0f);
    Vec2 diff;
    
    if (charCmp && Character::player == charCmp) {
        Vec2 mousePos(InputManager::GetInstance().GetMouseX() + Camera::pos.x, InputManager::GetInstance().GetMouseY() + Camera::pos.y);
        diff = mousePos - center;
        angle = diff.Inclination();
    } else {
        diff = Vec2(cos(angle), sin(angle));
    }
    
    associated.angleDeg = angle * 180.0f / 3.14159265f;

    // Posiciona arma a uma distancia do personagem
    Vec2 offset(diff.GetNormalized() * 30.0f);
    associated.box.x += offset.x;
    associated.box.y += offset.y;

    if (associated.angleDeg > 90.0f || associated.angleDeg < -90.0f) {
        if (associated.GetComponent<SpriteRenderer>()) {
            associated.GetComponent<SpriteRenderer>()->SetFlip(SDL_FLIP_VERTICAL);
        }
    } else {
        if (associated.GetComponent<SpriteRenderer>()) {
            associated.GetComponent<SpriteRenderer>()->SetFlip(SDL_FLIP_NONE);
        }
    }

    if (cooldownState > 0) {
        cdTimer.Update(dt);
        Animator* anim = associated.GetComponent<Animator>();

        if (cooldownState == 1 && cdTimer.Get() > 0.3f) {
            cooldownState = 2;
            cdTimer.Restart();
            reloadSound->Play(1);
            if(anim) anim->SetAnimation("reloading");
        } else if (cooldownState == 2 && cdTimer.Get() > 0.4f) {
            cooldownState = 3;
            cdTimer.Restart();
            if(anim) anim->SetAnimation("idle");
        } else if (cooldownState == 3 && cdTimer.Get() > 0.2f) {
            cooldownState = 0;
            cdTimer.Restart();
        }
    }
}

void Gun::Render() {}

void Gun::Shoot(Vec2 target) {
    if (cooldownState == 0) {
        Vec2 center(associated.box.x + associated.box.w / 2.0f, associated.box.y + associated.box.h / 2.0f);
        Vec2 direction = target - center;
        angle = direction.Inclination();
        associated.angleDeg = angle * 180.0f / 3.14159265f;
        
        shotSound->Play(1);
        cooldownState = 1;
        cdTimer.Restart();

        GameObject* bulletGo = new GameObject();
        bulletGo->box.x = center.x; // Centraliza a criacao
        bulletGo->box.y = center.y;
        
        bool targetsPlayer = true;
        std::shared_ptr<GameObject> charPtr = character.lock();
        if (charPtr && charPtr->GetComponent<Character>() == Character::player) {
            targetsPlayer = false;
        }

        Bullet* bullet = new Bullet(*bulletGo, angle, 400.0f, 10, 800.0f, targetsPlayer, charPtr);
        bulletGo->AddComponent(bullet);
        
        Game::GetInstance().GetCurrentState().AddObject(bulletGo);
    }
}
