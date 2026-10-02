#pragma once
#include "Component.h"
#include "Sprite.h"
#include "GameObject.h"

class SpriteRenderer : public Component {
private:
    Sprite sprite;
    bool cameraFollower;

public:
    SDL_RendererFlip flip;

    SpriteRenderer(GameObject& associated);
    SpriteRenderer(GameObject& associated, const std::string& file, int frameCountW = 1, int frameCountH = 1);
    
    void Open(const std::string& file);
    void SetFrameCount(int frameCountW, int frameCountH);
    void SetFrame(int frame);
    
    void SetCameraFollower(bool follower);
    bool IsCameraFollower();

    void SetFlip(SDL_RendererFlip flip);
    void SetScale(float scaleX, float scaleY);
    Vec2 GetScale();

    void Update(float dt) override;
    void Render() override;
};
