#include "Sound.h"
#include "Resources.h"
#include <iostream>

Sound::Sound(GameObject& associated) : Component(associated), chunk(nullptr), channel(-1) {
}

Sound::Sound(GameObject& associated, std::string file) : Sound(associated) {
    Open(file);
}

Sound::~Sound() {
    if (chunk != nullptr && channel != -1) {
        Mix_HaltChannel(channel);
    }
}

void Sound::Play(int times) {
    channel = Mix_PlayChannel(-1, chunk, times - 1);
}

void Sound::Stop() {
    if (chunk != nullptr && channel != -1) {
        Mix_HaltChannel(channel);
    }
}

void Sound::Open(std::string file) {
    chunk = Resources::GetSound(file);
    if (chunk == nullptr) {
        std::cerr << "Erro ao carregar o audio: " << file << ". " << SDL_GetError() << std::endl;
    }
}

bool Sound::IsOpen() {
    return chunk != nullptr;
}

void Sound::Update(float dt) {
}

void Sound::Render() {
}
