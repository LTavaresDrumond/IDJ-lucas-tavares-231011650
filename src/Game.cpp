#include "Game.h"
#include "State.h"
#include "Resources.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game* Game::instance = nullptr;

Game& Game::GetInstance() {
    if (instance == nullptr) {
        instance = new Game("Lucas Tavares Drumond 231011650", 1200, 900);
    }
    return *instance;
}

Game::Game(std::string title, int width, int height) : storedState(nullptr), frameStart(0), dt(0) {
    srand(time(NULL));
    if (instance != nullptr) {
        std::cerr << "Erro: Uma instância do jogo já está em execução!" << std::endl;
        return;
    }
    instance = this;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        std::cerr << "Falha na inicialização da SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        std::cerr << "Falha na inicialização da SDL_Image: " << IMG_GetError() << std::endl;
        exit(1);
    }

    int mixFlags = MIX_INIT_OGG | MIX_INIT_MP3;
    if ((Mix_Init(mixFlags) & mixFlags) != mixFlags) {
        std::cerr << "Falha na inicialização da SDL_Mixer: " << Mix_GetError() << std::endl;
        exit(1);
    }
    
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
        std::cerr << "Falha ao abrir o áudio: " << Mix_GetError() << std::endl;
        exit(1);
    }
    Mix_AllocateChannels(32);

    if (TTF_Init() != 0) {
        std::cerr << "Falha na inicialização da SDL_ttf: " << TTF_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        std::cerr << "Falha ao criar a janela: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Falha ao criar o renderizador: " << SDL_GetError() << std::endl;
        exit(1);
    }
}

Game::~Game() {
    if (storedState != nullptr) {
        delete storedState;
        storedState = nullptr;
    }
    while (!stateStack.empty()) {
        stateStack.pop();
    }
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
    Resources::ClearFonts();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

State& Game::GetCurrentState() {
    return *stateStack.top();
}

SDL_Renderer* Game::GetRenderer() {
    return renderer;
}

void Game::Push(State* state) {
    storedState = state;
}

void Game::CalculateDeltaTime() {
    int newTicks = SDL_GetTicks();
    dt = (newTicks - frameStart) / 1000.0f;
    frameStart = newTicks;
}

float Game::GetDeltaTime() {
    return dt;
}

void Game::Run() {
    InputManager& input = InputManager::GetInstance();

    // Empilhar o estado inicial (que foi Push-ado pela main)
    if (storedState != nullptr) {
        stateStack.emplace(storedState);
        storedState = nullptr;
        stateStack.top()->Start();
    } else {
        return; // Sem estado inicial, não roda
    }

    while (!stateStack.empty() && !stateStack.top()->QuitRequested()) {
        // Gerenciar a pilha: verificar se o estado atual quer ser desempilhado
        if (stateStack.top()->PopRequested()) {
            stateStack.pop();
            if (!stateStack.empty()) {
                stateStack.top()->Resume();
            }
        }

        // Se há um estado armazenado, empilhá-lo
        if (storedState != nullptr) {
            if (!stateStack.empty()) {
                stateStack.top()->Pause();
            }
            stateStack.emplace(storedState);
            storedState = nullptr;
            stateStack.top()->Start();
        }

        // Se a pilha ficou vazia após o pop, sair
        if (stateStack.empty()) break;

        CalculateDeltaTime();
        input.Update();

        stateStack.top()->Update(dt);
        SDL_RenderClear(renderer);
        stateStack.top()->Render();
        SDL_RenderPresent(renderer);
        SDL_Delay(33);
    }

    // Limpar a pilha antes de liberar recursos
    while (!stateStack.empty()) {
        stateStack.pop();
    }
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
    Resources::ClearFonts();
}
