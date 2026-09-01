#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"
#include "Music.h"

class State {
public:
    State();
    ~State();
    bool QuitRequested();
    void LoadAssets();
    void Update(float dt);
    void Render();
    void AddObject(GameObject* go);

private:
    Music music;
    bool quitRequested;
    std::vector<std::unique_ptr<GameObject>> objectArray;
};
