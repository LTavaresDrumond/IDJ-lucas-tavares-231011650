#include "Animator.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

Animator::Animator(GameObject& associated) : Component(associated), frameStart(0), frameEnd(0), frameTime(0), currentFrame(0), timeElapsed(0) {
}

void Animator::Update(float dt) {
    if (frameTime == 0.0f) return;

    timeElapsed += 1.0f; // Incremento manual conforme PDF pois dt ainda não está sendo passado

    if (timeElapsed > frameTime) {
        currentFrame++;
        timeElapsed -= frameTime;

        if (currentFrame > frameEnd) {
            currentFrame = frameStart;
        }

        SpriteRenderer* sr = associated.GetComponent<SpriteRenderer>();
        if (sr != nullptr) {
            sr->SetFrame(currentFrame);
        }
    }
}

void Animator::Render() {
}

void Animator::SetAnimation(std::string name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        frameStart = it->second.frameStart;
        frameEnd = it->second.frameEnd;
        frameTime = it->second.frameTime;
        currentFrame = frameStart;
        timeElapsed = 0.0f;

        SpriteRenderer* sr = associated.GetComponent<SpriteRenderer>();
        if (sr != nullptr) {
            sr->SetFrame(currentFrame);
        }
    }
}

void Animator::AddAnimation(std::string name, Animation anim) {
    auto it = animations.find(name);
    if (it == animations.end()) {
        animations[name] = anim;
    }
}
