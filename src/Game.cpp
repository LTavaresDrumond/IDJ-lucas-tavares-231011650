#include "Game.h"
#include <iostream>

Game* Game::instance = nullptr;

Game& Game::GetInstance() {
    if (instance == nullptr) {
        // A janela deve ter as dimensões 1200x900, e o título deve ser seu nome e matrícula.
        instance = new Game("Lucas Tavares Drumond 231011650", 1200, 900);
    }
    return *instance;
}

Game::Game(std::string title, int width, int height) {
    if (instance != nullptr) {
        std::cerr << "Erro: Uma instância do jogo já está em execução!" << std::endl;
        return;
    }
    instance = this;

    // Inicialização da SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        std::cerr << "Falha na inicialização da SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Inicialização da SDL_Image
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        std::cerr << "Falha na inicialização da SDL_Image: " << IMG_GetError() << std::endl;
        exit(1);
    }

    // Inicialização da SDL_Mixer
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

    // Criação da janela
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        std::cerr << "Falha ao criar a janela: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Criação do renderizador (Tenta acelerado primeiro, conforme as instruções do trabalho)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        // Fallback: se não achar renderizador acelerado (ex: rodando em modo dummy no Linux), tenta o de software
        std::cerr << "Aviso: Renderizador acelerado não encontrado. Tentando renderizador de software." << std::endl;
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == nullptr) {
            std::cerr << "Falha ao criar o renderizador: " << SDL_GetError() << std::endl;
            exit(1);
        }
    }

    // Inicializa o State
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

void Game::Run() {
    while (!state->QuitRequested()) {
        state->Update(0.0f);
        state->Render();
        SDL_RenderPresent(renderer);
        SDL_Delay(33); // Aproximadamente 30 FPS
    }
}
