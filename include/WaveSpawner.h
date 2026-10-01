#pragma once
#include "Component.h"
#include "Timer.h"
#include <vector>

struct Wave {
    int zombies;
    int npcs;
    float cooldown;
};

class WaveSpawner : public Component {
private:
    std::vector<Wave> waves;
    int currentWave;
    int zombiesToSpawn;
    int npcsToSpawn;
    Timer spawnTimer;
    
public:
    WaveSpawner(GameObject& associated);
    void Start() override;
    void Update(float dt) override;
    void Render() override;
};
