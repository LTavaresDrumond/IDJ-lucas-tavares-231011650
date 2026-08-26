#include "Music.h"
#include <iostream>

Music::Music() {
    music = nullptr;
}

Music::Music(std::string file) {
    music = nullptr;
    Open(file);
}

Music::~Music() {
    Stop();
    if (music != nullptr) {
        Mix_FreeMusic(music);
    }
}

void Music::Play(int times) {
    if (music != nullptr) {
        Mix_PlayMusic(music, times);
    }
}

void Music::Stop(int msToStop) {
    Mix_FadeOutMusic(msToStop);
}

void Music::Open(std::string file) {
    music = Mix_LoadMUS(file.c_str());
    if (music == nullptr) {
        std::cerr << "Erro ao carregar música " << file << ": " << SDL_GetError() << std::endl;
    }
}

bool Music::IsOpen() {
    return music != nullptr;
}
