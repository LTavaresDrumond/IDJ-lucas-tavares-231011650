#include "Animator.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

Animator::Animator(GameObject& associated) : Component(associated), frameStart(0), frameEnd(0), frameTime(0), currentFrame(0), timeElapsed(0), current(""), flip(SDL_FLIP_NONE) {
}

void Animator::Update(float dt) {
    if (frameTime == 0.0f) return;

    timeElapsed += dt; // Incremento baseado no delta time

    if (timeElapsed > frameTime) {
        currentFrame++;
        timeElapsed -= frameTime;

        if (currentFrame > frameEnd) {
            currentFrame = frameStart;
        }

        SpriteRenderer* sr = associated.GetComponent<SpriteRenderer>();
        if (sr != nullptr) {
            sr->SetFrame(currentFrame);
            sr->SetFlip(flip);
        }
    }
}

void Animator::Render() {
}

void Animator::SetAnimation(std::string name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        if (current == name && frameStart == it->second.frameStart && frameEnd == it->second.frameEnd && flip == it->second.flip) return;

        frameStart = it->second.frameStart;
        frameEnd = it->second.frameEnd;
        frameTime = it->second.frameTime;
        currentFrame = frameStart;
        timeElapsed = 0.0f;
        flip = it->second.flip;
        current = name;

        SpriteRenderer* sr = associated.GetComponent<SpriteRenderer>();
        if (sr != nullptr) {
            sr->SetFrame(currentFrame);
            sr->SetFlip(flip);
        }
    }
}

void Animator::AddAnimation(std::string name, Animation anim) {
    animations[name] = anim;
}
