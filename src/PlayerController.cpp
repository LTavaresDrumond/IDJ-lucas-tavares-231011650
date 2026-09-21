#include "PlayerController.h"
#include "Character.h"
#include "InputManager.h"
#include "Camera.h"

PlayerController::PlayerController(GameObject& associated) : Component(associated) {
}

void PlayerController::Update(float dt) {
    if (Character::player) {
        InputManager& input = InputManager::GetInstance();
        
        bool w = input.IsKeyDown('w');
        bool s = input.IsKeyDown('s');
        bool a = input.IsKeyDown('a');
        bool d = input.IsKeyDown('d');
        
        Vec2 moveDir(0, 0);
        if (w) moveDir.y -= 1;
        if (s) moveDir.y += 1;
        if (a) moveDir.x -= 1;
        if (d) moveDir.x += 1;

        if (moveDir.x != 0 || moveDir.y != 0) {
            moveDir = moveDir.GetNormalized();
            Character::player->Issue(Character::Command(Character::MOVE, moveDir.x, moveDir.y));
        }

        if (input.MousePress(LEFT_MOUSE_BUTTON)) {
            Vec2 target(input.GetMouseX() + Camera::pos.x, input.GetMouseY() + Camera::pos.y);
            Character::player->Issue(Character::Command(Character::SHOOT, target.x, target.y));
        }
    }
}

void PlayerController::Render() {}
