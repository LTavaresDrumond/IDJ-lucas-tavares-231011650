#pragma once
#include <string>

#define INCLUDE_SDL
#include "SDL_include.h"

class Sprite {
public:
    Sprite();
    Sprite(std::string file, int frameCountW = 1, int frameCountH = 1);
    ~Sprite();
    void Open(std::string file);
    void SetClip(int x, int y, int w, int h);
    void Render(int x, int y, int w, int h);
    void Render(int x, int y); // Antigo para não quebrar outras coisas, ou adaptar o novo
    void SetFrame(int frame);
    void SetFrameCount(int frameCountW, int frameCountH);
    int GetWidth();
    int GetHeight();
    bool IsOpen();

private:
    SDL_Texture* texture;
    int width;
    int height;
    SDL_Rect clipRect;
    int frameCountW;
    int frameCountH;
};
