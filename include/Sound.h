#pragma once
#include "Component.h"
#include <string>

#define INCLUDE_SDL
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

class Sound : public Component {
private:
    Mix_Chunk* chunk;
    int channel;

public:
    Sound(GameObject& associated);
    Sound(GameObject& associated, std::string file);
    ~Sound();
    void Play(int times = 1);
    void Stop();
    void Open(std::string file);
    bool IsOpen();
    
    void Update(float dt) override;
    void Render() override;
};
