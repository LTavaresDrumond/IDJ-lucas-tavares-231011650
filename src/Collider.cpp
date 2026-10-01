#include "Collider.h"
#include "GameObject.h"

#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#endif // DEBUG

#ifndef PI
#define PI 3.14159265358979323846f
#endif

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset) 
    : Component(associated), scale(scale), offset(offset) {
}

void Collider::Start() {
    // Inicializa box baseando-se na box do GameObject
    box = associated.box;
}

void Collider::Update(float dt) {
    box = associated.box;

    // Aplica a escala em torno do centro
    Vec2 center = box.GetCenter();
    box.w *= scale.x;
    box.h *= scale.y;
    box.SetCenter(center + offset);
}

void Collider::Render() {
#ifdef DEBUG
	Vec2 center( box.GetCenter() );
	SDL_Point points[5];

	Vec2 point = (Vec2(box.x, box.y) - center).GetRotated( associated.angleDeg * PI / 180.0f )
					+ center - Camera::pos;
	points[0] = {(int)point.x, (int)point.y};
	points[4] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x + box.w, box.y) - center).GetRotated( associated.angleDeg * PI / 180.0f )
					+ center - Camera::pos;
	points[1] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x + box.w, box.y + box.h) - center).GetRotated( associated.angleDeg * PI / 180.0f )
					+ center - Camera::pos;
	points[2] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x, box.y + box.h) - center).GetRotated( associated.angleDeg * PI / 180.0f )
					+ center - Camera::pos;
	points[3] = {(int)point.x, (int)point.y};

	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

void Collider::SetScale(Vec2 scale) {
    this->scale = scale;
}

void Collider::SetOffset(Vec2 offset) {
    this->offset = offset;
}
