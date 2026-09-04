#include "Game.h"
#include "Resources.h"
#include <iostream>

Game* Game::instance = nullptr;

Game& Game::GetInstance() {
    if (instance == nullptr) {
        instance = new Game("Lucas Tavares Drumond 231011650", 1200, 900);
    }
    return *instance;
}

Game::Game(std::string title, int width, int height) : frameStart(0), dt(0) {
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

    state = new State();
}

Game::~Game() {
    delete state;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

State& Game::GetState() {
    return *state;
}

SDL_Renderer* Game::GetRenderer() {
    return renderer;
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

    while (!state->QuitRequested() && !input.QuitRequested()) {
        CalculateDeltaTime();
        input.Update();

        state->Update(dt);
        SDL_RenderClear(renderer);
        state->Render();
        SDL_RenderPresent(renderer);
        SDL_Delay(33); 
    }
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
}
