#include "WaveSpawner.h"
#include "Zombie.h"
#include "Collider.h"
#include "State.h"
#include "Game.h"
#include "Camera.h"
#include "Character.h"
#include "AIController.h"
#include <cstdlib>

WaveSpawner::WaveSpawner(GameObject& associated) : Component(associated), currentWave(0) {
    waves.push_back({3, 0, 2.0f});
    waves.push_back({5, 1, 1.5f});
    waves.push_back({10, 2, 1.0f});
    waves.push_back({15, 3, 0.8f}); // Wave 4: 15 zombies, 3 npcs
    
    if (waves.size() > 0) {
        zombiesToSpawn = waves[0].zombies;
        npcsToSpawn = waves[0].npcs;
    } else {
        zombiesToSpawn = 0;
        npcsToSpawn = 0;
    }
}

void WaveSpawner::Start() {
    // Retirado o spawn fixo do start.
}

void WaveSpawner::Update(float dt) {
    if (currentWave >= (int)waves.size()) return; // Acabaram as waves

    if (zombiesToSpawn > 0 || npcsToSpawn > 0) {
        spawnTimer.Update(dt);
        if (spawnTimer.Get() >= waves[currentWave].cooldown) {
            spawnTimer.Restart();
            
            // Decidir se spawna zombie ou NPC (prioridade pro zombie)
            bool spawnZombie = false;
            if (zombiesToSpawn > 0 && npcsToSpawn > 0) {
                spawnZombie = (rand() % 2 == 0); // 50% de chance para cada se ambos precisarem
            } else if (zombiesToSpawn > 0) {
                spawnZombie = true;
            } else if (npcsToSpawn > 0) {
                spawnZombie = false;
            }

            // Achar posição distante
            float x, y;
            do {
                x = rand() % 2560; // 40 tiles * 64px
                y = rand() % 2560;
            } while (Vec2(x, y).Distance(Camera::pos) < 800.0f); // Nasce longe do player
            
            if (spawnZombie) {
                GameObject* zombieObj = new GameObject();
                zombieObj->box.x = x;
                zombieObj->box.y = y;
                
                Zombie* zombieComp = new Zombie(*zombieObj);
                zombieObj->AddComponent(zombieComp);
                
                Collider* zombieCol = new Collider(*zombieObj);
                zombieObj->AddComponent(zombieCol);
                
                Game::GetInstance().GetCurrentState().AddObject(zombieObj);
                zombiesToSpawn--;
            } else {
                GameObject* npcObj = new GameObject();
                npcObj->box.x = x;
                npcObj->box.y = y;
                
                Character* charComp = new Character(*npcObj, "Recursos/img/NPC.png");
                npcObj->AddComponent(charComp);
                
                AIController* aiComp = new AIController(*npcObj);
                npcObj->AddComponent(aiComp);
                
                Collider* colComp = new Collider(*npcObj);
                npcObj->AddComponent(colComp);
                
                Game::GetInstance().GetCurrentState().AddObject(npcObj);
                npcsToSpawn--;
            }
        }
    } else {
        // Se todos da wave nasceram, espera morrerem para avançar
        if (Zombie::aliveCount == 0) {
            currentWave++;
            if (currentWave < (int)waves.size()) {
                zombiesToSpawn = waves[currentWave].zombies;
                npcsToSpawn = waves[currentWave].npcs;
                spawnTimer.Restart();
            }
        }
    }
}

void WaveSpawner::Render() {}

bool WaveSpawner::IsFinished() {
    return currentWave >= (int)waves.size() && Zombie::aliveCount == 0 && zombiesToSpawn == 0;
}
