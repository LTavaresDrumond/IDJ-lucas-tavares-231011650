#pragma once

// Forward declaration para evitar dependência circular
class GameObject;

class Component {
protected:
    GameObject& associated;

public:
    Component(GameObject& associated);
    virtual ~Component();

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
};
