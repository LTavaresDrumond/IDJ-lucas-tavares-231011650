#include "Character.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Game.h"

Character* Character::player = nullptr;

Character::Character(GameObject& associated, std::string sprite) 
    : Component(associated), speed(0,0), hp(100), linearSpeed(200.0f) {
    player = this;

    // MEGA BRAIN 2.0: Player.png is an RPG sprite with 3 columns and 4 rows!
    SpriteRenderer* sr = new SpriteRenderer(associated, sprite, 3, 4);
    associated.AddComponent(sr);

    Animator* anim = new Animator(associated);
    anim->AddAnimation("idle", Animation(1, 1, 0.0f));
    anim->AddAnimation("walking", Animation(0, 2, 0.15f));
    anim->AddAnimation("dead", Animation(0, 0, 0.1f)); 
    associated.AddComponent(anim);
}

Character::~Character() {
    if (player == this) {
        player = nullptr;
    }
}

void Character::Start() {
}

void Character::Update(float dt) {
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
            moved = true;
        } else if (cmd.type == SHOOT) {
            // TODO: Gun Shoot
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
            } else { // Up (Row 3)
                anim->AddAnimation("walking", Animation(9, 11, 0.15f, SDL_FLIP_NONE));
                anim->AddAnimation("idle",    Animation(10, 10, 0.0f, SDL_FLIP_NONE));
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
