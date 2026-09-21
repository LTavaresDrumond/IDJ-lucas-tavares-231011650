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
    void Start();
    void Update(float dt);
    void Render();
    std::weak_ptr<GameObject> AddObject(GameObject* go);
    std::weak_ptr<GameObject> GetObjectPtr(GameObject* go);

private:
    Music music;
    bool quitRequested;
    bool started;
    std::vector<std::shared_ptr<GameObject>> objectArray;
};
