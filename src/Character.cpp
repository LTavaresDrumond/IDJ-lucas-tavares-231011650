#include "Character.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Game.h"
#include "Gun.h"
#include "Camera.h"
#include "Bullet.h"
#include "Zombie.h"
#include "Collider.h"

Character* Character::player = nullptr;

Character::Character(GameObject& associated, std::string sprite) 
    : Component(associated), speed(0,0), hp(100), linearSpeed(200.0f), 
      hitSound(associated, "Recursos/audio/Hit1.wav"), deathSound(associated, "Recursos/audio/Dead.wav") {

    // MEGA BRAIN 2.0: Player.png is an RPG sprite with 3 columns and 4 rows!
    SpriteRenderer* sr = new SpriteRenderer(associated, sprite, 3, 4);
    associated.AddComponent(sr);

    Animator* anim = new Animator(associated);
    anim->AddAnimation("idle", Animation(1, 1, 0.0f));
    anim->AddAnimation("walking", Animation(0, 2, 0.15f));
    anim->AddAnimation("dead", Animation(10, 10, 0.1f)); // Frame da lápide na última linha
    associated.AddComponent(anim);
}

Character::~Character() {
    if (player == this) {
        player = nullptr;
    }
}

void Character::Start() {
    GameObject* gunGo = new GameObject();
    std::shared_ptr<GameObject> characterPtr = Game::GetInstance().GetState().GetObjectPtr(&associated).lock();
    Gun* gunComp = new Gun(*gunGo, characterPtr);
    gunGo->AddComponent(gunComp);
    gun = Game::GetInstance().GetState().AddObject(gunGo);
}

void Character::Update(float dt) {
    damageCooldown.Update(dt);

    if (hp <= 0) {
        associated.GetComponent<Animator>()->SetAnimation("dead");
        deathTimer.Update(dt);
        if (deathTimer.Get() > 2.0f) { 
            associated.RequestDelete();
        }
        return;
    }

    bool moved = false;

    while (!taskQueue.empty()) {
        Command cmd = taskQueue.front();
        if (cmd.type == MOVE) {
            speed = cmd.pos * linearSpeed;
            associated.box.x += speed.x * dt;
            associated.box.y += speed.y * dt;
            
            // Limitar ao mapa (40 tiles * 64px = 2560px)
            float mapSize = 2560.0f;
            if (associated.box.x < 0) associated.box.x = 0;
            if (associated.box.y < 0) associated.box.y = 0;
            if (associated.box.x + associated.box.w > mapSize) associated.box.x = mapSize - associated.box.w;
            if (associated.box.y + associated.box.h > mapSize) associated.box.y = mapSize - associated.box.h;
            
            moved = true;
        } else if (cmd.type == SHOOT) {
            std::shared_ptr<GameObject> gunPtr = gun.lock();
            if (gunPtr) {
                Gun* gunComp = gunPtr->GetComponent<Gun>();
                if (gunComp) {
                    gunComp->Shoot(cmd.pos);
                }
            }
        }
        taskQueue.pop(); // Remove action after executing
    }

    Animator* anim = associated.GetComponent<Animator>();
    if (moved) {
        if (std::abs(speed.x) > std::abs(speed.y)) {
            if (speed.x > 0) { // Right (Row 2)
                anim->AddAnimation("walking", Animation(6, 8, 0.15f, SDL_FLIP_NONE));
                anim->AddAnimation("idle",    Animation(7, 7, 0.0f,  SDL_FLIP_NONE));
            } else { // Left (Row 2 espelhado)
                anim->AddAnimation("walking", Animation(6, 8, 0.15f, SDL_FLIP_HORIZONTAL));
                anim->AddAnimation("idle",    Animation(7, 7, 0.0f,  SDL_FLIP_HORIZONTAL));
            }
        } else {
            if (speed.y > 0) { // Down (Row 0)
                anim->AddAnimation("walking", Animation(0, 2, 0.15f, SDL_FLIP_NONE));
                anim->AddAnimation("idle",    Animation(1, 1, 0.0f,  SDL_FLIP_NONE));
            } else { // Up (Mesmo do Down, já que não tem sprite para cima)
                anim->AddAnimation("walking", Animation(0, 2, 0.15f, SDL_FLIP_NONE));
                anim->AddAnimation("idle",    Animation(1, 1, 0.0f,  SDL_FLIP_NONE));
            }
        }
        anim->SetAnimation("walking");
    } else {
        anim->SetAnimation("idle");
    }
}

void Character::Render() {}

void Character::Issue(Command task) {
    taskQueue.push(task);
}

void Character::NotifyCollision(GameObject& other) {
    if (hp <= 0) return;

    bool takeDamage = false;
    Bullet* bullet = other.GetComponent<Bullet>();
    Zombie* zombie = other.GetComponent<Zombie>();

    if (bullet != nullptr) {
        if (player == this && bullet->targetsPlayer) {
            takeDamage = true;
        } else if (player != this && !bullet->targetsPlayer) {
            takeDamage = true;
        }
        
        if (takeDamage) {
            hp -= bullet->GetDamage();
        }
    } else if (zombie != nullptr) {
        if (player == this && damageCooldown.Get() > 1.0f) {
            takeDamage = true;
            hp -= 10; // Zumbis tiram 10 de vida do jogador no toque
            damageCooldown.Restart();
        }
    }

    if (takeDamage) {
        if (hp <= 0) {
            deathSound.Play(1);
            if (player == this) {
                Camera::Unfollow();
            }

            Collider* col = associated.GetComponent<Collider>();
            if (col != nullptr) {
                associated.RemoveComponent(col);
                delete col;
            }

            std::shared_ptr<GameObject> g = gun.lock();
            if (g) {
                g->RequestDelete();
            }
        } else {
            hitSound.Play(1);
        }
    }
}
