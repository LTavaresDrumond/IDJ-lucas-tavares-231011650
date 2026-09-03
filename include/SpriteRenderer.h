#pragma once
#include "Component.h"
#include "Sprite.h"
#include "GameObject.h"

class SpriteRenderer : public Component {
private:
    Sprite sprite;
    bool cameraFollower;

public:
    SpriteRenderer(GameObject& associated);
    SpriteRenderer(GameObject& associated, std::string file, int frameCountW = 1, int frameCountH = 1);
    
    void Open(std::string file);
    void SetFrameCount(int frameCountW, int frameCountH);
    void SetFrame(int frame);
    
    void SetCameraFollower(bool follower);
    bool IsCameraFollower();

    void Update(float dt) override;
    void Render() override;
};
