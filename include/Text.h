#pragma once

#include "Component.h"
#include <string>

#define INCLUDE_SDL_TTF
#include "SDL_include.h"

class Text : public Component {
public:
    enum TextStyle { SOLID, SHADED, BLENDED };

    Text(GameObject& associated, const std::string& fontFile, int fontSize, TextStyle style, const std::string& text, SDL_Color color);
    ~Text();

    void Update(float dt) override;
    void Render() override;

    void SetText(const std::string& text);
    void SetColor(SDL_Color color);
    void SetStyle(TextStyle style);
    void SetFontFile(const std::string& fontFile);
    void SetFontSize(int fontSize);

private:
    void RemakeTexture();

    TTF_Font* font;
    SDL_Texture* texture;
    std::string text;
    TextStyle style;
    std::string fontFile;
    int fontSize;
    SDL_Color color;
};
