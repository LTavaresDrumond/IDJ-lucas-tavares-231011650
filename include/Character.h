#pragma once
#include "Component.h"
#include "Vec2.h"
#include "Timer.h"
#include "Sound.h"
#include <string>
#include <queue>
#include <memory>

class Character : public Component {
public:
    enum CommandType { MOVE, SHOOT };

    class Command {
    public:
        CommandType type;
        Vec2 pos;
        Command(CommandType type, float x, float y) : type(type), pos(x, y) {}
    };

    static Character* player;

    Character(GameObject& associated, std::string sprite);
    ~Character();

    GameObject& GetAssociated() { return associated; }

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    void NotifyCollision(GameObject& other) override;
    void Issue(Command task);

    Vec2 speed;
    int hp;

private:
    std::weak_ptr<GameObject> gun;
    std::queue<Command> taskQueue;
    float linearSpeed;
    Timer deathTimer;
    Timer damageCooldown;
    Sound hitSound;
    Sound deathSound;
};
