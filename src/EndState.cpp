#include "EndState.h"
#include "TitleState.h"
#include "GameData.h"
#include "SpriteRenderer.h"
#include "Text.h"
#include "Game.h"
#include "InputManager.h"
#include "Camera.h"

EndState::EndState() : showText(true) {
    GameObject* bgObj = new GameObject();
    if (GameData::playerVictory) {
        SpriteRenderer* bgRenderer = new SpriteRenderer(*bgObj, "Recursos/img/Win.png");
        bgObj->AddComponent(bgRenderer);
        backgroundMusic.Open("Recursos/audio/endStateWin.ogg");
    } else {
        SpriteRenderer* bgRenderer = new SpriteRenderer(*bgObj, "Recursos/img/Lose.png");
        bgObj->AddComponent(bgRenderer);
        backgroundMusic.Open("Recursos/audio/endStateLose.ogg");
    }
    AddObject(bgObj);

    GameObject* textObj = new GameObject();
    SDL_Color color = {255, 255, 255, 255};
    
    // The instruction text
    Text* textRenderer = new Text(*textObj, "Recursos/font/neodgm.ttf", 40, Text::BLENDED, 
        "Pressione ESPACO para retornar ao menu ou ESC para sair", color);
    textObj->AddComponent(textRenderer);
    
    // Center it on screen
    textObj->box.x = 1200 / 2.0f - textObj->box.w / 2.0f;
    textObj->box.y = 900 / 2.0f + 350.0f;
    AddObject(textObj);
}

EndState::~EndState() {
    objectArray.clear();
}

void EndState::LoadAssets() {}

void EndState::Start() {
    LoadAssets();
    StartArray();
    Camera::pos = Vec2(0,0);
    backgroundMusic.Play(1);
}

void EndState::Pause() {}

void EndState::Resume() {
    Camera::pos = Vec2(0,0);
}

void EndState::Update(float dt) {
    InputManager& input = InputManager::GetInstance();
    
    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
    }

    if (input.KeyPress(SPACE_KEY)) {
        popRequested = true;
    }

    textTimer.Update(dt);
    if (textTimer.Get() > 0.5f) {
        showText = !showText;
        textTimer.Restart();
    }

    UpdateArray(dt);
}

void EndState::Render() {
    objectArray[0]->Render();
    if (showText) {
        objectArray[1]->Render();
    }
}
