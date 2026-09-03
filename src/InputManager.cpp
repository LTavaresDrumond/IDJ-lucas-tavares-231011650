#include "InputManager.h"
#include <cstring>

InputManager::InputManager() : quitRequested(false), updateCounter(0), mouseX(0), mouseY(0) {
    memset(mouseState, 0, sizeof(mouseState));
    memset(mouseUpdate, 0, sizeof(mouseUpdate));
}

InputManager::~InputManager() {
}

InputManager& InputManager::GetInstance() {
    static InputManager instance;
    return instance;
}

void InputManager::Update() {
    SDL_Event event;
    
    SDL_GetMouseState(&mouseX, &mouseY);
    quitRequested = false;
    updateCounter++;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quitRequested = true;
        } else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
            int button = event.button.button;
            bool state = (event.type == SDL_MOUSEBUTTONDOWN);
            mouseState[button] = state;
            mouseUpdate[button] = updateCounter;
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            if (event.key.repeat != 1) {
                int key = event.key.keysym.sym;
                bool state = (event.type == SDL_KEYDOWN);
                keyState[key] = state;
                keyUpdate[key] = updateCounter;
            }
        }
    }
}

bool InputManager::KeyPress(int key) {
    return keyState[key] && (keyUpdate[key] == updateCounter);
}

bool InputManager::KeyRelease(int key) {
    return !keyState[key] && (keyUpdate[key] == updateCounter);
}

bool InputManager::IsKeyDown(int key) {
    return keyState[key];
}

bool InputManager::MousePress(int button) {
    return mouseState[button] && (mouseUpdate[button] == updateCounter);
}

bool InputManager::MouseRelease(int button) {
    return !mouseState[button] && (mouseUpdate[button] == updateCounter);
}

bool InputManager::IsMouseDown(int button) {
    return mouseState[button];
}

int InputManager::GetMouseX() {
    return mouseX;
}

int InputManager::GetMouseY() {
    return mouseY;
}

bool InputManager::QuitRequested() {
    return quitRequested;
}
