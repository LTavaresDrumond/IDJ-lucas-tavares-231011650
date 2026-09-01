#include "Sprite.h"
#include "Game.h"
#include <iostream>

Sprite::Sprite() : texture(nullptr), width(0), height(0), frameCountW(1), frameCountH(1) {
}

Sprite::Sprite(std::string file, int frameCountW, int frameCountH) 
    : texture(nullptr), width(0), height(0), frameCountW(frameCountW), frameCountH(frameCountH) {
    Open(file);
}

Sprite::~Sprite() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

void Sprite::Open(std::string file) {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    if (texture == nullptr) {
        std::cerr << "Erro ao carregar textura " << file << ": " << SDL_GetError() << std::endl;
        return;
    }

    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SetFrame(0);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

void Sprite::Render(int x, int y, int w, int h) {
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;

    SDL_RenderCopy(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect);
}

void Sprite::Render(int x, int y) {
    Render(x, y, clipRect.w, clipRect.h);
}

void Sprite::SetFrame(int frame) {
    int frameW = GetWidth();
    int frameH = GetHeight();
    
    int row = frame / frameCountW;
    int col = frame % frameCountW;

    int x = col * frameW;
    int y = row * frameH;

    // Check if inside image
    if (x + frameW <= width && y + frameH <= height) {
        SetClip(x, y, frameW, frameH);
    }
}

void Sprite::SetFrameCount(int frameCountW, int frameCountH) {
    this->frameCountW = frameCountW;
    this->frameCountH = frameCountH;
}

int Sprite::GetWidth() {
    return width / frameCountW;
}

int Sprite::GetHeight() {
    return height / frameCountH;
}

bool Sprite::IsOpen() {
    return texture != nullptr;
}
