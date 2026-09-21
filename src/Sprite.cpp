#include "Sprite.h"
#include "Resources.h"
#include "Game.h"
#include <iostream>

Sprite::Sprite() : texture(nullptr), width(0), height(0), frameCountW(1), frameCountH(1), scale(1.0f, 1.0f) {
}

Sprite::Sprite(std::string file, int frameCountW, int frameCountH) 
    : texture(nullptr), width(0), height(0), frameCountW(frameCountW), frameCountH(frameCountH), scale(1.0f, 1.0f) {
    Open(file);
}

Sprite::~Sprite() {
    // Destrutor vazio pois Resources cuida da desalocação
}

void Sprite::Open(std::string file) {
    texture = Resources::GetImage(file);
    
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

void Sprite::Render(int x, int y, int w, int h, float angle, SDL_RendererFlip flip) {
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;

    SDL_Point center = {w/2, h/2};
    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect, angle, &center, flip);
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

void Sprite::SetScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}

Vec2 Sprite::GetScale() {
    return scale;
}

int Sprite::GetWidth() {
    return (width / frameCountW) * scale.x;
}

int Sprite::GetHeight() {
    return (height / frameCountH) * scale.y;
}

bool Sprite::IsOpen() {
    return texture != nullptr;
}
