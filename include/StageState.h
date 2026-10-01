#pragma once

#include "State.h"
#include "Music.h"

class TileSet;

class StageState : public State {
public:
    StageState();
    ~StageState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;
    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    TileSet* tileSet;
    Music backgroundMusic;
    std::weak_ptr<GameObject> spawner;
};
