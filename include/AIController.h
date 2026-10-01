#pragma once
#include "Component.h"
#include "Timer.h"
#include "Vec2.h"

class AIController : public Component {
public:
    enum AIState { MOVING, RESTING };

    AIController(GameObject& associated);
    void Update(float dt) override;
    void Render() override;

private:
    AIState state;
    Timer timer;
    Vec2 targetPos;
};
