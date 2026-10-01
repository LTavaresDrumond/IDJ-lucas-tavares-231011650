#pragma once
#include <string>
#include <stack>
#include <memory>

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "SDL_include.h"

#include "InputManager.h"

class State;

class Game {
private:
    Game(std::string title, int width, int height);
    static Game* instance;
    SDL_Window* window;
    SDL_Renderer* renderer;
    State* storedState;

    int frameStart;
    float dt;

    void CalculateDeltaTime();

    std::stack<std::unique_ptr<State>> stateStack;

public:
    ~Game();
    void Run();
    SDL_Renderer* GetRenderer();
    State& GetCurrentState();
    static Game& GetInstance();
    float GetDeltaTime();
    void Push(State* state);
};
