#include "Text.h"
#include "Resources.h"
#include "Game.h"
#include "Camera.h"
#include <iostream>

Text::Text(GameObject& associated, std::string fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color)
    : Component(associated), font(nullptr), texture(nullptr), text(text), style(style), fontFile(fontFile), fontSize(fontSize), color(color) {
    RemakeTexture();
}

Text::~Text() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

void Text::Update(float dt) {
}

void Text::Render() {
    if (texture != nullptr) {
        SDL_Rect srcRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = associated.box.w;
        srcRect.h = associated.box.h;

        SDL_Rect dstRect;
        dstRect.x = associated.box.x - Camera::pos.x;
        dstRect.y = associated.box.y - Camera::pos.y;
        dstRect.w = associated.box.w;
        dstRect.h = associated.box.h;

        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &srcRect, &dstRect, associated.angleDeg, nullptr, SDL_FLIP_NONE);
    }
}

void Text::SetText(std::string text) {
    this->text = text;
    RemakeTexture();
}

void Text::SetColor(SDL_Color color) {
    this->color = color;
    RemakeTexture();
}

void Text::SetStyle(TextStyle style) {
    this->style = style;
    RemakeTexture();
}

void Text::SetFontFile(std::string fontFile) {
    this->fontFile = fontFile;
    RemakeTexture();
}

void Text::SetFontSize(int fontSize) {
    this->fontSize = fontSize;
    RemakeTexture();
}

void Text::RemakeTexture() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    font = Resources::GetFont(fontFile, fontSize);
    if (font == nullptr) return;

    SDL_Surface* surface = nullptr;
    if (style == SOLID) {
        surface = TTF_RenderText_Solid(font, text.c_str(), color);
    } else if (style == SHADED) {
        SDL_Color bg = {0, 0, 0, 255}; // Fundo preto
        surface = TTF_RenderText_Shaded(font, text.c_str(), color, bg);
    } else if (style == BLENDED) {
        surface = TTF_RenderText_Blended(font, text.c_str(), color);
    }

    if (surface == nullptr) {
        std::cerr << "Falha ao renderizar texto: " << TTF_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
    if (texture == nullptr) {
        std::cerr << "Falha ao criar textura de texto: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    associated.box.w = surface->w;
    associated.box.h = surface->h;
    
    SDL_FreeSurface(surface);
}
