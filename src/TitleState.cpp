#include "TitleState.h"
#include "StageState.h"
#include "SpriteRenderer.h"
#include "Text.h"
#include "InputManager.h"
#include "Game.h"
#include "Camera.h"

TitleState::TitleState() : showText(true) {
    GameObject* bgObj = new GameObject();
    SpriteRenderer* bgRenderer = new SpriteRenderer(*bgObj, "Recursos/img/Title.png");
    bgObj->AddComponent(bgRenderer);
    AddObject(bgObj);

    GameObject* textObj = new GameObject();
    SDL_Color color = {255, 255, 255, 255}; // Branco
    Text* textRenderer = new Text(*textObj, "Recursos/font/neodgm.ttf", 60, Text::BLENDED, "Press Space to continue", color);
    textObj->AddComponent(textRenderer);
    
    // Centralizar o texto na tela. As dimensões foram preenchidas no RemakeTexture.
    textObj->box.x = 1200 / 2.0f - textObj->box.w / 2.0f;
    textObj->box.y = 900 / 2.0f + 250.0f; 
    AddObject(textObj);
}

TitleState::~TitleState() {
    objectArray.clear();
}

void TitleState::LoadAssets() {
}

void TitleState::Start() {
    LoadAssets();
    StartArray();
    Camera::pos = Vec2(0, 0);
}

void TitleState::Pause() {
}

void TitleState::Resume() {
    Camera::pos = Vec2(0, 0);
}

void TitleState::Update(float dt) {
    InputManager& input = InputManager::GetInstance();
    
    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
    }

    if (input.KeyPress(SPACE_KEY)) {
        Game::GetInstance().Push(new StageState());
    }

    textTimer.Update(dt);
    if (textTimer.Get() > 0.5f) {
        showText = !showText;
        textTimer.Restart();
    }

    UpdateArray(dt);
}

void TitleState::Render() {
    // Renderiza o BG
    objectArray[0]->Render();

    // Renderiza o Texto piscante
    if (showText) {
        objectArray[1]->Render();
    }
}
