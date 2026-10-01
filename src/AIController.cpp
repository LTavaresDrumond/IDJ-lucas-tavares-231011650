#include "AIController.h"
#include "Character.h"
#include "GameObject.h"

AIController::AIController(GameObject& associated) : Component(associated), state(RESTING) {
    timer.Restart();
}

void AIController::Update(float dt) {
    if (Character::player == nullptr) return;

    Character* charComp = associated.GetComponent<Character>();
    if (charComp == nullptr || charComp->hp <= 0) return;

    timer.Update(dt);

    if (state == RESTING) {
        if (timer.Get() > 1.0f) { // Cooldown de descanso
            state = MOVING;
            timer.Restart();
        }
    } else if (state == MOVING) {
        Vec2 playerPos = Character::player->GetAssociated().box.GetCenter();
        Vec2 myPos = associated.box.GetCenter();

        float distanceToPlayer = myPos.Distance(playerPos);
        
        // Se está longe do player, se aproxima
        if (distanceToPlayer > 400.0f) {
            Vec2 dir = (playerPos - myPos).GetNormalized();
            charComp->Issue(Character::Command(Character::MOVE, dir.x, dir.y));
        } else {
            // Está perto do player. Atira na direção dele!
            charComp->Issue(Character::Command(Character::SHOOT, playerPos.x, playerPos.y));
            state = RESTING;
            timer.Restart();
        }
    }
}

void AIController::Render() {}
