#include "Camera.h"
#include "InputManager.h"
#include "Game.h"

GameObject* Camera::focus = nullptr;
Vec2 Camera::pos = Vec2(0, 0);
Vec2 Camera::speed = Vec2(0, 0);

void Camera::Follow(GameObject* newFocus) {
    focus = newFocus;
}

void Camera::Unfollow() {
    focus = nullptr;
}

void Camera::Update(float dt) {
    if (focus != nullptr) {
        // Assume 1200x900 resolution based on Game initialization
        pos.x = focus->box.x - 1200 / 2.0f;
        pos.y = focus->box.y - 900 / 2.0f;
    } else {
        InputManager& input = InputManager::GetInstance();
        
        speed = Vec2(0, 0);
        float moveSpeed = 300.0f;

        if (input.IsKeyDown(LEFT_ARROW_KEY)) {
            speed.x = -moveSpeed;
        }
        if (input.IsKeyDown(RIGHT_ARROW_KEY)) {
            speed.x = moveSpeed;
        }
        if (input.IsKeyDown(UP_ARROW_KEY)) {
            speed.y = -moveSpeed;
        }
        if (input.IsKeyDown(DOWN_ARROW_KEY)) {
            speed.y = moveSpeed;
        }

        pos.x += speed.x * dt;
        pos.y += speed.y * dt;
    }
}
