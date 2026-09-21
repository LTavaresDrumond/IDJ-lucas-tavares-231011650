#pragma once
#include "Component.h"

class PlayerController : public Component {
public:
    PlayerController(GameObject& associated);
    void Update(float dt) override;
    void Render() override;
};
