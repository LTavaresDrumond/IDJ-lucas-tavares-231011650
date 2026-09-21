#pragma once
#define INCLUDE_SDL
#include "SDL_include.h"

class Animation {
public:
    int frameStart;
    int frameEnd;
    float frameTime;
    SDL_RendererFlip flip;

    Animation() : frameStart(0), frameEnd(0), frameTime(0), flip(SDL_FLIP_NONE) {}
    Animation(int frameStart, int frameEnd, float frameTime, SDL_RendererFlip flip = SDL_FLIP_NONE) 
        : frameStart(frameStart), frameEnd(frameEnd), frameTime(frameTime), flip(flip) {}
};
