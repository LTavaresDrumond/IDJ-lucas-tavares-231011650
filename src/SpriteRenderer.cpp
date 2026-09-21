#include "SpriteRenderer.h"
#include "Camera.h"

SpriteRenderer::SpriteRenderer(GameObject& associated) : Component(associated), sprite(), cameraFollower(false), flip(SDL_FLIP_NONE) {
}

SpriteRenderer::SpriteRenderer(GameObject& associated, std::string file, int frameCountW, int frameCountH) 
    : Component(associated), sprite(file, frameCountW, frameCountH), cameraFollower(false), flip(SDL_FLIP_NONE) {
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

void SpriteRenderer::Open(std::string file) {
    sprite.Open(file);
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

void SpriteRenderer::SetFrameCount(int frameCountW, int frameCountH) {
    sprite.SetFrameCount(frameCountW, frameCountH);
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

void SpriteRenderer::SetFrame(int frame) {
    sprite.SetFrame(frame);
}

void SpriteRenderer::SetCameraFollower(bool follower) {
    cameraFollower = follower;
}

bool SpriteRenderer::IsCameraFollower() {
    return cameraFollower;
}

void SpriteRenderer::Update(float dt) {
}

void SpriteRenderer::Render() {
    int renderX = associated.box.x - (cameraFollower ? 0 : Camera::pos.x);
    int renderY = associated.box.y - (cameraFollower ? 0 : Camera::pos.y);
    sprite.Render(renderX, renderY, associated.box.w, associated.box.h, associated.angleDeg, flip);
}

void SpriteRenderer::SetFlip(SDL_RendererFlip flip) {
    this->flip = flip;
}

void SpriteRenderer::SetScale(float scaleX, float scaleY) {
    sprite.SetScale(scaleX, scaleY);
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

Vec2 SpriteRenderer::GetScale() {
    return sprite.GetScale();
}
